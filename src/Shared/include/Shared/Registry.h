#pragma once

#include <cassert>
#include <mutex>
#include <functional>
#include <iostream>
#include <unordered_set>

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
                    std::cerr << e.what() << std::endl;
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
          : _mutex(),
            _objects()
        {

        }

        ~Registry()
        {
            assert(_objects.empty());
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
            // TODO: Add to separate collection if ForEach is running
            // and have ForEach do additions afterwards
            // Maybe always register to separate collection and
            // apply to real collection at beginning/end of ForEach.
            // Or maybe have ForEach take a copy of the collection
            // and iterate the copy.
            std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
            if(!lock.owns_lock()){
                throw std::logic_error("Calling Registry::Register() from ForEach "
                    "callback is currently not supported");
            }
            _objects.insert(&obj);
        }

        inline void Deregister(T& obj)
        {
            // TODO: Add to separate collection if ForEach is running
            // and have ForEach do removals afterward
            // Maybe always deregister to separate collection and
            // apply to real collection at beginning/end of ForEach.
            // Or maybe have ForEach take a copy of the collection
            // and iterate the copy.
            std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
            if(!lock.owns_lock()){
                throw std::logic_error("Calling Registry::Deregister() from ForEach "
                    "callback is currently not supported");
            }
            _objects.erase(&obj);
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            // TODO: Re-entrant calls to ForEach should be allowed
            std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
            if(!lock.owns_lock()){
                throw std::logic_error("Nested call to Registry::ForEach() from ForEach "
                    "callback is currently not supported");
            }
            for(auto* obj : _objects)
            {
                callback(*obj);
            }
        }

    private:
        std::mutex _mutex;
        std::unordered_set<T*> _objects;
    };
}

