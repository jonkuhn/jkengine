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
        class Registration
        {
        public:
            Registration(Registry<T>& registry, T& obj)
              : _registry(registry),
                _obj(obj)
            {
                _registry.Register(_obj);
            }

            ~Registration()
            {
                try
                {
                    _registry.Deregister(_obj);
                }
                catch (std::exception &e)
                {
                    std::cerr << "FATAL: Caught exception during Registry::Registration "
                        << "destructor: " << e.what() << std::endl;
                    std::abort();
                }
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

        private:
            Registry<T>& _registry;
            T& _obj;
        };

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
        std::mutex _objectsMutex;
        std::unordered_set<T*> _objects;
    };
}

