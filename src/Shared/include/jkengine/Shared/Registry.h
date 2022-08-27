#pragma once

#include <cstdlib>
#include <bitset>
#include <iostream>
#include <mutex>
#include <sstream>

namespace Shared
{
    // TODO: consider renaming Deregister() to Destroy or Destruct or something.
    class IDeregisterable
    {
    public:
        virtual void Deregister(void* pointer) = 0;
    };

    class RegDeleter
    {
    public:
        RegDeleter()
            : _deregisterable(nullptr)
        {

        }

        RegDeleter(IDeregisterable& deregisterable)
            : _deregisterable(&deregisterable)
        {

        }

        void operator()(void* pointer)
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: RegistryDeleter called with nullptr" << std::endl;
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
    class RegUniquePtr
    {
    public:
        typedef typename std::unique_ptr<TInterface, RegDeleter> T;

        RegUniquePtr() = delete;
    };

    // TODO: Rename to Pool
    template<typename T, size_t ObjectsPerMemoryChunk = 128>
    class Registry final : public IDeregisterable
    {
    public:
        Registry()
          : _mutex(),
            _isIndexAllocated(),
            _memoryChunk(new (std::align_val_t(alignof(T))) uint8_t[sizeof(T) * ObjectsPerMemoryChunk]),
            _objects(static_cast<T*>(_memoryChunk))
        {

        }

        ~Registry()
        {
            ::operator delete[] (static_cast<uint8_t*>(_memoryChunk), std::align_val_t(alignof(T))) ;
            if(_isIndexAllocated.any())
            {
                std::cerr << "FATAL: Registry destructor called with "
                    << _isIndexAllocated.count() << " objects still registered."
                    << std::endl;
                std::abort();
            }
        }

        // Do not allow copy.  If it were copyable then registrations
        // would only de-register from the original not the copy.
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        // Do not allow move.  If it were moveable then the registrations
        // would need to somehow be updated to point at the destination
        // registry rather than the source.
        Registry(Registry&&) = delete;
        Registry& operator=(Registry&&) = delete;

        template<typename TInterface = T, typename ... Args>
        inline typename RegUniquePtr<TInterface>::T MakeUnique(Args&&... args)
        {
            void* memoryForObj = nullptr;

            {
                std::scoped_lock<std::mutex> lock(_mutex);

                for (size_t i = 0; i < ObjectsPerMemoryChunk; i++)
                {
                    if (!_isIndexAllocated[i])
                    {
                        memoryForObj = &_objects[i];
                        _isIndexAllocated[i] = true;
                        break;
                    }
                }
            }

            if (memoryForObj == nullptr)
            {
                // TODO: automatically allocate new chunk of memory
                throw std::logic_error("Pool is full");
            }

            // TODO: consider clearing _isIndexAllocated[i] if the following
            // throws by using an RAII object to set _isIndexAllocated[i]

            typename RegUniquePtr<TInterface>::T objUniquePointer(
                new (memoryForObj) T(std::forward<Args>(args)...),
                RegDeleter(*this));

            return objUniquePointer;
        }

        void Deregister(void* pointer) override
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: Registry::Deregister called with nullptr" << std::endl;
                std::abort();
            }

            auto* pointerAsBytePtr = static_cast<uint8_t*>(pointer);
            auto* startOfChunk = static_cast<uint8_t*>(_memoryChunk);
            auto* pastEndOfChunk = startOfChunk + ObjectsPerMemoryChunk * sizeof(T);
            size_t index = (pointerAsBytePtr - startOfChunk) / sizeof(T);

            if ((pointerAsBytePtr < startOfChunk)
                || ((pointerAsBytePtr + sizeof(T) - 1) >= pastEndOfChunk)
                || index >= ObjectsPerMemoryChunk)
            {
                std::stringstream ss;
                ss << "Deregister called for object that is out of range: "
                    << " _objects = 0x" << std::hex << reinterpret_cast<uintptr_t>(_objects)
                    << " startOfChunk = 0x" << std::hex << reinterpret_cast<uintptr_t>(startOfChunk)
                    << " pastEndOfChunk = 0x" << std::hex << reinterpret_cast<uintptr_t>(pastEndOfChunk)
                    << " pointer = 0x" << std::hex << reinterpret_cast<uintptr_t>(pointer)
                    << " sizeof(T) = " << std::dec << sizeof(T)
                    << " alignof(T) = " << std::dec << alignof(T)
                    << " index = " << std::dec << index;
                throw std::out_of_range(ss.str().c_str());
            }

            T& obj = *static_cast<T*>(pointer);
            obj.~T();

            std::scoped_lock<std::mutex> lock(_mutex);
            _isIndexAllocated[index] = false;
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            for (size_t i = 0; i < ObjectsPerMemoryChunk; i++)
            {
                {
                    std::scoped_lock<std::mutex> lock(_mutex);
                    if (!_isIndexAllocated[i])
                    {
                        continue;
                    }
                }
                callback(_objects[i]);
            }
        }

    private:
        std::mutex _mutex;
        std::bitset<ObjectsPerMemoryChunk> _isIndexAllocated;
        void* const _memoryChunk;
        T* const _objects;
    };
}

