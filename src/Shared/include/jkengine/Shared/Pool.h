#pragma once

#include <chrono>
#include <cstdlib>
#include <bitset>
#include <iostream>
#include <mutex>
#include <numeric>
#include <sstream>
#include <vector>

namespace Shared
{
    class IDestroyer
    {
    public:
        virtual void Destroy(void* pointer) = 0;
    };

    class PoolDeleter
    {
    public:
        PoolDeleter()
            : _destroyer(nullptr)
        {

        }

        PoolDeleter(IDestroyer& destroyer)
            : _destroyer(&destroyer)
        {

        }

        void operator()(void* pointer)
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: PoolDeleter called with nullptr" << std::endl;
                std::abort();
            }

            if (_destroyer != nullptr)
            {
                _destroyer->Destroy(pointer);
            }

        }

    private:
        IDestroyer* _destroyer;
    };

    template<typename TInterface>
    class PoolUniquePtr
    {
    public:
        typedef typename std::unique_ptr<TInterface, PoolDeleter> T;

        PoolUniquePtr() = delete;
    };

    template<typename T, int CapacityPerChunk>
    class MemoryChunk
    {
    public:
        MemoryChunk()
          : _isIndexAllocated(),
            _freeIndices(),
            _memoryChunk(new (std::align_val_t(alignof(T))) uint8_t[sizeof(T) * CapacityPerChunk]),
            _objects(reinterpret_cast<T*>(_memoryChunk))
        {
            _freeIndices.reserve(CapacityPerChunk);
            for (int i = (CapacityPerChunk-1); i >= 0; i--)
            {
                _freeIndices.push_back(i);
            }
        }

        ~MemoryChunk()
        {
            if (_memoryChunk != nullptr)
            {
                ::operator delete[] (static_cast<uint8_t*>(_memoryChunk), std::align_val_t(alignof(T))) ;
            }
        }

        // Non-copyable
        MemoryChunk(const MemoryChunk&) = delete;
        MemoryChunk& operator=(const MemoryChunk&) = delete;

        MemoryChunk(MemoryChunk&& other) noexcept
          : _isIndexAllocated(std::move(other._isIndexAllocated)),
            _freeIndices(std::move(other._freeIndices)),
            _memoryChunk(std::exchange(other._memoryChunk, nullptr)),
            _objects(std::exchange(other._objects, nullptr))
        {

        };

        MemoryChunk& operator=(MemoryChunk&& other) noexcept
        {
            std::swap(_isIndexAllocated, other._isIndexAllocated);
            std::swap(_freeIndices, other._freeIndices);
            std::swap(_memoryChunk, other._memoryChunk);
            std::swap(_objects, other._objects);
            return *this;
        };

        inline T* ObjectAt(int index)
        {
            if (_isIndexAllocated[index])
            {
                return &(_objects[index]);
            }
            return nullptr;
        }

        inline int CountAllocatedIndices()
        {
            return CapacityPerChunk - _freeIndices.size();
        }

        inline bool IsPointerInRange(void* pointer)
        {
            uint8_t* pointerAsBytePtr = static_cast<uint8_t*>(pointer);
            uint8_t* startOfChunk = _memoryChunk;
            uint8_t* pastEndOfChunk = startOfChunk + CapacityPerChunk * sizeof(T);

            if ((pointerAsBytePtr >= startOfChunk)
                && ((pointerAsBytePtr + sizeof(T)) <= pastEndOfChunk))
            {
                return true;
            }

            return false;
        }

        template<typename TInterface = T, typename ... Args>
        inline typename PoolUniquePtr<TInterface>::T MakeUnique(IDestroyer& destroyer, Args&&... args)
        {
            void* memoryForObj = TryToFindMemoryForNewObject();
            if (memoryForObj == nullptr)
            {
                return nullptr;
            }

            typename PoolUniquePtr<TInterface>::T objUniquePointer(
                new (memoryForObj) T(std::forward<Args>(args)...),
                PoolDeleter(destroyer));

            return objUniquePointer;
        }

        inline void Destroy(T* object)
        {
            object->~T();

            uint8_t* pointerAsBytePtr = reinterpret_cast<uint8_t*>(object);
            uint8_t* startOfChunk = _memoryChunk;
            int index = (pointerAsBytePtr - startOfChunk) / sizeof(T);
            _freeIndices.push_back(index);
            _isIndexAllocated[index] = false;
        }

        inline std::string DebugString(void* pointer)
        {
            uint8_t* pointerAsBytePtr = static_cast<uint8_t*>(pointer);
            uint8_t* startOfChunk = _memoryChunk;
            uint8_t* pastEndOfChunk = startOfChunk + CapacityPerChunk * sizeof(T);
            int index = (pointerAsBytePtr - startOfChunk) / sizeof(T);

            std::stringstream ss;
            ss << " MemoryChunk(_objects = 0x" << std::hex << reinterpret_cast<uintptr_t>(_objects)
                << ", startOfChunk = 0x" << std::hex << reinterpret_cast<uintptr_t>(startOfChunk)
                << ", pastEndOfChunk = 0x" << std::hex << reinterpret_cast<uintptr_t>(pastEndOfChunk)
                << ", index = " << std::dec << index << ")";

            return ss.str();
        }

    private:
        inline void* TryToFindMemoryForNewObject()
        {
            if (_freeIndices.empty())
            {
                return nullptr;
            }

            int index = _freeIndices.back();
            _freeIndices.pop_back();
            _isIndexAllocated[index] = true;

            return &_objects[index];
        }

        std::bitset<CapacityPerChunk> _isIndexAllocated;
        std::vector<int> _freeIndices;
        uint8_t* _memoryChunk;
        T* _objects;
    };

    template<typename T, int CapacityPerChunk = 128>
    class Pool final : public IDestroyer
    {
    public:
        Pool()
          : _mutex(),
            _memoryChunks(1)
        {

        }

        ~Pool()
        {
            std::scoped_lock<std::mutex> lock(_mutex);
            int countStillAllocated = 0;
            for(auto& memoryChunk : _memoryChunks)
            {
                countStillAllocated += memoryChunk.CountAllocatedIndices();
            }

            if (countStillAllocated > 0)
            {
                std::cerr << "FATAL: Pool destructor called with "
                    << countStillAllocated << " objects still allocated."
                    << std::endl;
                std::abort();
            }
        }

        // Do not allow copy.  If it were copyable then PoolDeleter
        // would only de-register from the original not the copy.
        Pool(const Pool&) = delete;
        Pool& operator=(const Pool&) = delete;

        // Do not allow move.  If it were moveable then the PoolDeleter
        // would need to somehow be updated to point at the destination
        // pool rather than the source.
        Pool(Pool&&) = delete;
        Pool& operator=(Pool&&) = delete;

        template<typename TInterface = T, typename ... Args>
        inline typename PoolUniquePtr<TInterface>::T MakeUnique(Args&&... args)
        {
            std::scoped_lock<std::mutex> lock(_mutex);
            for (auto& memoryChunk : _memoryChunks)
            {
                auto newObject = memoryChunk.template MakeUnique<TInterface>(*this, std::forward<Args>(args)...);
                if (newObject)
                {
                    return newObject;
                }
            }

            _memoryChunks.emplace_back();
            return _memoryChunks.back().template MakeUnique<TInterface>(*this, std::forward<Args>(args)...);
        }

        void Destroy(void* pointer) override
        {
            std::scoped_lock<std::mutex> lock(_mutex);
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: Pool::Destroy called with nullptr" << std::endl;
                std::abort();
            }

            for (auto& memoryChunk : _memoryChunks)
            {
                if (!memoryChunk.IsPointerInRange(pointer))
                {
                    // keep looking for the right chunk
                    continue;
                }
                memoryChunk.Destroy(static_cast<T*>(pointer));
                return;
            }

            std::stringstream ss;
            ss << "Destroy called for object that is out of range: "
                << " pointer = 0x" << std::hex << reinterpret_cast<uintptr_t>(pointer)
                << " sizeof(T) = " << std::dec << sizeof(T)
                << " alignof(T) = " << std::dec << alignof(T)
                << std::endl;

            for (auto& memoryChunk : _memoryChunks)
            {
                ss << memoryChunk.DebugString(pointer) << std::endl;
            }

            throw std::out_of_range(ss.str().c_str());
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            // This is safe because we never decrease the size of 
            // _memoryChunks so all indices less than this will be
            // valid.
            int memoryChunkCount = 0;
            {
                std::scoped_lock<std::mutex> lock(_mutex);
                memoryChunkCount = _memoryChunks.size();
            }

            for (int chunk = 0; chunk < memoryChunkCount; chunk++)
            {
                for (int index = 0; index < CapacityPerChunk; index++)
                {
                    T* obj = nullptr;
                    {
                        std::scoped_lock<std::mutex> lock(_mutex);
                        obj = _memoryChunks[chunk].ObjectAt(index);

                        if (obj == nullptr)
                        {
                            // no object at the given index, keep looking
                            continue;
                        }
                    }

                    // Objects allocated during the callback may or may not
                    // have the callback called for them.
                    callback(*obj);
                }
            }
        }

        std::mutex _mutex;
        std::vector<MemoryChunk<T, CapacityPerChunk>> _memoryChunks;
    };
}
