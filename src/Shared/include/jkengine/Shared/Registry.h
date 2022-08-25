#pragma once

#include <cstdlib>
#include <mutex>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

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

    template<typename T>
    class Registry final : public IDeregisterable
    {
    public:
        Registry()
          : _objectsMutex(),
            _objects()
        {

        }

        ~Registry()
        {
            if(!_objects.empty())
            {
                std::cerr << "FATAL: Registry destructor called with "
                    << _objects.size() << " objects still registered."
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

        // TODO: make real code use this and also figure out how to
        // return something that wraps unique_ptr from the user-facing
        // interfaces from the engine.
        template<typename TInterface = T, typename ... Args>
        inline typename RegUniquePtr<TInterface>::T MakeUnique(Args&&... args)
        {
            void* memory = new (std::align_val_t(alignof(T))) uint8_t[sizeof(T)];
            T* objRawPointer = new (memory) T(std::forward<Args>(args)...);
            typename RegUniquePtr<TInterface>::T objUniquePointer(objRawPointer, RegDeleter(*this));
            Register(*objRawPointer);
            return objUniquePointer;
        }

        void Deregister(void* pointer) override
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: Registry::Deregister called with nullptr" << std::endl;
                std::abort();
            }
            
            T& obj = *static_cast<T*>(pointer);
            obj.~T();

            std::scoped_lock<std::mutex> lock(_objectsMutex);
            _objects.erase(&obj);

            ::operator delete[] (static_cast<uint8_t*>(pointer), std::align_val_t(alignof(T))) ;
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            std::unordered_set<T*> localCopyOfObjects;
            {
                std::scoped_lock<std::mutex> lock(_objectsMutex);
                localCopyOfObjects = _objects;
            }

            for(auto* obj : localCopyOfObjects)
            {
                callback(*obj);
            }
        }

    private:
        inline void Register(T& obj)
        {
            std::scoped_lock<std::mutex> lock(_objectsMutex);
            _objects.insert(&obj);
        }

        std::mutex _objectsMutex;

        // TODO: allocate a big block of memory that can hold some
        // number of properly aligned T objects
        std::unordered_set<T*> _objects;
    };
}

