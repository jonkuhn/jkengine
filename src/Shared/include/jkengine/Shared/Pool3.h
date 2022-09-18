#pragma once

#include <algorithm>
#include <memory>
#include <vector>

namespace Shared
{
    class IPool3
    {
    public:
        virtual void* Get(size_t index) = 0;
        virtual void Destroy(size_t index) = 0;
    };

    template<typename TInterface>
    class PoolUniquePtr3
    {
    public:
        PoolUniquePtr3()
            : _pool(nullptr),
              _index(0)
        {

        }

        PoolUniquePtr3(IPool3& pool, size_t index)
            : _pool(&pool),
              _index(index)
        {

        }

        ~PoolUniquePtr3()
        {
            if (_pool != nullptr)
            {
                _pool->Destroy(_index);
            }
        }

        // Do not allow copy.
        PoolUniquePtr3(const PoolUniquePtr3&) = delete;
        PoolUniquePtr3& operator=(const PoolUniquePtr3&) = delete;

        PoolUniquePtr3(PoolUniquePtr3&& other)
          : _pool(std::move(other._pool)),
            _index(other._index)
        {

        }

        PoolUniquePtr3& operator=(PoolUniquePtr3&& rhs)
        {
            std::swap(_pool, rhs._pool);
            std::swap(_index, rhs._index);
        }

        TInterface& operator*() const
        {
            // TODO: this may be a bad idea because the pointers returned
            // from here can be invalidated if the pool is modified
            return *(static_cast<TInterface*>(_pool->Get(_index)));
        }

        TInterface* operator->() const
        {
            return static_cast<TInterface*>(_pool->Get(_index));
        }

    private:
        IPool3* _pool;
        size_t _index;
    };

    template<typename T>
    class Pool3 final : public IPool3
    {
    public:
        Pool3()
          : _objects()
        {

        }

        // Do not allow copy.  If it were copyable then PoolDeleter
        // would only de-register from the original not the copy.
        Pool3(const Pool3&) = delete;
        Pool3& operator=(const Pool3&) = delete;

        // Do not allow move.  If it were moveable then the PoolDeleter
        // would need to somehow be updated to point at the destination
        // Pool3 rather than the source.
        Pool3(Pool3&&) = delete;
        Pool3& operator=(Pool3&&) = delete;

        template<typename TInterface = T, typename ... Args>
        inline PoolUniquePtr3<TInterface> MakeUnique(Args&&... args)
        {
            int index = _objects.size();
            _objects.emplace_back(std::forward<Args>(args)...);
            return PoolUniquePtr3<TInterface>(*this, index);
        }

        virtual void* Get(size_t index) override
        {
            return static_cast<void*>(&_objects[index]);
        }

        virtual void Destroy(size_t index) override
        {
            if (index > _objects.size())
            {
                std::cerr << "FATAL: Pool::Destroy called with nullptr" << std::endl;
                std::abort();
            }

            _objects.erase(_objects.begin() + index);
        }

        typedef std::function<void(T&)> ForEachCallback;
        inline void ForEach(ForEachCallback callback)
        {
            for (auto& obj : _objects)
            {
                callback(obj);
            }
        }
        
    private:
        std::vector<T> _objects;
    };
}
