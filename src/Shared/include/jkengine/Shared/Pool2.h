#pragma once

#include <algorithm>
#include <memory>
#include <vector>

namespace Shared
{
    class IDestroyer2
    {
    public:
        virtual void Destroy(void* pointer) = 0;
    };

    class PoolDeleter2
    {
    public:
        PoolDeleter2()
            : _destroyer(nullptr)
        {

        }

        PoolDeleter2(IDestroyer2& destroyer)
            : _destroyer(&destroyer)
        {

        }

        void operator()(void* pointer)
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: PoolDeleter2 called with nullptr" << std::endl;
                std::abort();
            }

            if (_destroyer != nullptr)
            {
                _destroyer->Destroy(pointer);
            }

        }

    private:
        IDestroyer2* _destroyer;
    };

    template<typename TInterface>
    class PoolUniquePtr2
    {
    public:
        typedef typename std::unique_ptr<TInterface, PoolDeleter2> T;

        PoolUniquePtr2() = delete;
    };

    template<typename T>
    class Pool2 final : public IDestroyer2
    {
    public:
        Pool2()
          : _objects()
        {

        }

        // Do not allow copy.  If it were copyable then PoolDeleter
        // would only de-register from the original not the copy.
        Pool2(const Pool2&) = delete;
        Pool2& operator=(const Pool2&) = delete;

        // Do not allow move.  If it were moveable then the PoolDeleter
        // would need to somehow be updated to point at the destination
        // Pool2 rather than the source.
        Pool2(Pool2&&) = delete;
        Pool2& operator=(Pool2&&) = delete;

        template<typename TInterface = T, typename ... Args>
        inline typename PoolUniquePtr2<TInterface>::T MakeUnique(Args&&... args)
        {
            std::unique_ptr<TInterface, PoolDeleter2> obj(
                new T(std::forward<Args>(args)...),
                PoolDeleter2(*this)
            );

            _objects.push_back(obj.get());

            return obj;
        }

        void Destroy(void* pointer) override
        {
            if (pointer == nullptr)
            {
                std::cerr << "FATAL: Pool::Destroy called with nullptr" << std::endl;
                std::abort();
            }

            auto foundIter = std::find(
                _objects.begin(),
                _objects.end(),
                static_cast<T*>(pointer));

            if (foundIter != _objects.end())
            {
                _objects.erase(foundIter);
                return;
            }

            std::stringstream ss;
            ss << "Destroy called for object that is out of range: "
                << " pointer = 0x" << std::hex << reinterpret_cast<uintptr_t>(pointer)
                << " sizeof(T) = " << std::dec << sizeof(T)
                << " alignof(T) = " << std::dec << alignof(T)
                << std::endl;

            throw std::out_of_range(ss.str().c_str());
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            for (auto* obj : _objects)
            {
                callback(*obj);
            }
        }
        
    private:
        std::vector<T*> _objects;
    };
}
