#pragma once

#include <cstdlib>
#include <bitset>
#include <iostream>
#include <mutex>
#include <sstream>
#include <vector>

namespace Shared
{
    // TODO: consider renaming Deregister() to Destroy or Destruct or something.
    class IDeregisterable
    {
    public:
        virtual void Deregister(void* pointer) = 0;
    };

    class PoolDeleter
    {
    public:
        PoolDeleter()
            : _deregisterable(nullptr)
        {

        }

        PoolDeleter(IDeregisterable& deregisterable)
            : _deregisterable(&deregisterable)
        {

        }

        void operator()(void* pointer)
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: PoolDeleter called with nullptr" << std::endl;
                std::abort();
            }

            if (_deregisterable != nullptr)
            {
                _deregisterable->Deregister(pointer);
            }

        }

    private:
        IDeregisterable* _deregisterable;
    };

    template<typename TInterface>
    class PoolUniquePtr
    {
    public:
        typedef typename std::unique_ptr<TInterface, PoolDeleter> T;

        PoolUniquePtr() = delete;
    };

    // TODO: Rename to Pool
    template<typename T, size_t CapacityPerChunk = 128>
    class Pool final : public IDeregisterable
    {
    public:
        Pool()
          : _mutex(),
            _isChunkIndexAllocated(),
            _memoryChunks(),
            _chunkObjects()
        {
            AllocateNewChunk();
        }

        ~Pool()
        {
            size_t countStillAllocated = 0;
            for(auto& isIndexAllocated : _isChunkIndexAllocated)
            {
                if(isIndexAllocated.any())
                {
                    countStillAllocated += isIndexAllocated.count();
                }
            }

            if (countStillAllocated)
            {
                std::cerr << "FATAL: Pool destructor called with "
                    << countStillAllocated << " objects still registered."
                    << std::endl;
                std::abort();
            }

            for(auto* memoryChunk : _memoryChunks)
            {
                ::operator delete[] (static_cast<uint8_t*>(memoryChunk), std::align_val_t(alignof(T))) ;
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
            void* memoryForObj = TryToFindMemoryForNewObject();
            if (memoryForObj == nullptr)
            {
                AllocateNewChunk();

                // TODO: optimize to not search again
                memoryForObj = TryToFindMemoryForNewObject();
            }

            // TODO: consider clearing _isIndexAllocated[i] if the following
            // throws by using an RAII object to set _isIndexAllocated[i]

            typename PoolUniquePtr<TInterface>::T objUniquePointer(
                new (memoryForObj) T(std::forward<Args>(args)...),
                PoolDeleter(*this));

            return objUniquePointer;
        }

        void Deregister(void* pointer) override
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: Pool::Deregister called with nullptr" << std::endl;
                std::abort();
            }

            for(size_t chunk = 0; chunk < _memoryChunks.size(); chunk++)
            {
                auto* pointerAsBytePtr = static_cast<uint8_t*>(pointer);
                auto* startOfChunk = _memoryChunks[chunk];
                auto* pastEndOfChunk = startOfChunk + CapacityPerChunk * sizeof(T);
                size_t index = (pointerAsBytePtr - startOfChunk) / sizeof(T);

                if ((pointerAsBytePtr < startOfChunk)
                    || ((pointerAsBytePtr + sizeof(T) - 1) >= pastEndOfChunk)
                    || index >= CapacityPerChunk)
                {
                    // keep looking for the right chunk
                    continue;
                }

                T& obj = *static_cast<T*>(pointer);
                obj.~T();

                std::scoped_lock<std::mutex> lock(_mutex);
                _isChunkIndexAllocated[chunk][index] = false;
                return;
            }

            std::stringstream ss;
            ss << "Deregister called for object that is out of range: "
                << " pointer = 0x" << std::hex << reinterpret_cast<uintptr_t>(pointer)
                << " sizeof(T) = " << std::dec << sizeof(T)
                << " alignof(T) = " << std::dec << alignof(T);

            for(size_t chunk = 0; chunk < _memoryChunks.size(); chunk++)
            {
                auto* pointerAsBytePtr = static_cast<uint8_t*>(pointer);
                auto* startOfChunk = _memoryChunks[chunk];
                auto* pastEndOfChunk = startOfChunk + CapacityPerChunk * sizeof(T);
                size_t index = (pointerAsBytePtr - startOfChunk) / sizeof(T);

                ss << " _chunkObjects[" << std::dec << chunk << "] = 0x" << std::hex << reinterpret_cast<uintptr_t>(_chunkObjects[chunk])
                    << " startOfChunk[" << std::dec << chunk << "] = 0x" << std::hex << reinterpret_cast<uintptr_t>(startOfChunk)
                    << " pastEndOfChunk[" << std::dec << chunk << "] = 0x" << std::hex << reinterpret_cast<uintptr_t>(pastEndOfChunk)
                    << " indexInChunk[" << std::dec << chunk << "] = " << std::dec << index;
            }

            throw std::out_of_range(ss.str().c_str());
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            // TODO: do we need to make sure _memoryChunks.size() gets re-evaluated each
            // loop and also should it be wrapped in a lock?
            for(size_t chunk = 0; chunk < _memoryChunks.size(); chunk++)
            {
                for (size_t i = 0; i < CapacityPerChunk; i++)
                {
                    T* obj = nullptr;
                    {
                        std::scoped_lock<std::mutex> lock(_mutex);
                        if (!_isChunkIndexAllocated[chunk][i])
                        {
                            continue;
                        }
                        obj = &_chunkObjects[chunk][i];
                    }
                    callback(*obj);
                }
            }
        }

    private:
        void AllocateNewChunk()
        {
            _isChunkIndexAllocated.emplace_back();

            uint8_t* newMemoryChunk = new (std::align_val_t(alignof(T))) uint8_t[sizeof(T) * CapacityPerChunk];
            _memoryChunks.push_back(newMemoryChunk);

            _chunkObjects.push_back({
                reinterpret_cast<T*>(newMemoryChunk)
            });

            if(!((_isChunkIndexAllocated.size() == _memoryChunks.size())
                && (_memoryChunks.size() == _chunkObjects.size())))
            {
                std::stringstream ss;
                ss << "Invariant violated not all chunk related vectors have the same size "
                    << " _isChunkIndexAllocated.size() = " << _isChunkIndexAllocated.size()
                    << ", _memoryChunks.size() = " << _memoryChunks.size()
                    << ", _chunkObjects.size() = " << _chunkObjects.size();
                throw std::logic_error(ss.str().c_str());
            }
        }

        void* TryToFindMemoryForNewObject()
        {
            std::scoped_lock<std::mutex> lock(_mutex);

            for(size_t chunk = 0; chunk < _isChunkIndexAllocated.size(); chunk++)
            {
                for (size_t i = 0; i < CapacityPerChunk; i++)
                {
                    if (!_isChunkIndexAllocated[chunk][i])
                    {
                        _isChunkIndexAllocated[chunk][i] = true;
                        return &_chunkObjects[chunk][i];
                    }
                }
            }

            return nullptr;
        }

        std::mutex _mutex;
        std::vector<std::bitset<CapacityPerChunk>> _isChunkIndexAllocated;
        std::vector<uint8_t*> _memoryChunks;
        std::vector<T*> _chunkObjects;
    };
}

