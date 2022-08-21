#pragma once

#include <cstdlib>
#include <mutex>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace Shared
{
    template<typename T>
    class Registry
    {
    public:
        // TODO: Remove this class
        class Registration
        {
        public:
            Registration(Registry<T>&, T&)
            {

            }

            // Do not allow copy.  Allowing a default copy would mean
            // objects would be de-registered twice.  Perhaps it could
            // make sense if the new copy registered itself so there
            // were two registrations.  However, it is unnecessary right
            // now for current use cases
            Registration(const Registration&) = delete;
            Registration& operator=(const Registration&) = delete;

            // Do not allow move.  Allowing a default move would mean 
            // objects would be de-registered twice (because the pointers
            // would remain in the source).  A move could be implemented
            // by nulling out the pointers in the source object.
            Registration(Registration&&) = delete;
            Registration& operator=(Registration&&) = delete;
        };

        class Deleter
        {
        public:
            Deleter()
              : _registry(nullptr)
            {

            }
            Deleter(Registry<T>& registry)
              : _registry(&registry)
            {

            }

            void operator()(T* object)
            {
                if (object == nullptr)
                {
                    std::cerr << "FATAL: RegistryDeleter called with nullptr" << std::endl;
                    std::abort();
                }

                if (_registry != nullptr)
                {
                    _registry->Deregister(*object);
                }
                delete object;
            }

        private:
            Registry<T>* _registry;
        };
        friend class Deleter;

        typedef std::unique_ptr<T, Deleter> UniquePtr;

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

        template<typename ... Args>
        inline UniquePtr MakeUnique(Args&&... args)
        {
            auto obj = UniquePtr(new T(*this, std::forward<Args>(args)...), Deleter(*this));
            Register(*obj);
            return obj;
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

        inline void Deregister(T& obj)
        {
            std::scoped_lock<std::mutex> lock(_objectsMutex);
            _objects.erase(&obj);
        }

        std::mutex _objectsMutex;
        std::unordered_set<T*> _objects;
    };
}

