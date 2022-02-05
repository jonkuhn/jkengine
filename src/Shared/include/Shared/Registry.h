#pragma once

#include <cassert>
#include <unordered_set>

namespace Graphics::OpenGL
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
                _registry.Deregister(_obj);
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
          : _objects()
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
            _objects.insert(&obj);
        }

        inline void Deregister(T& obj)
        {
            _objects.erase(&obj);
        }

        inline typename std::unordered_set<T*>::iterator begin()
        {
            return _objects.begin();
        }

        inline typename std::unordered_set<T*>::iterator end()
        {
            return _objects.end();
        }

    private:
        std::unordered_set<T*> _objects;
    };
}

