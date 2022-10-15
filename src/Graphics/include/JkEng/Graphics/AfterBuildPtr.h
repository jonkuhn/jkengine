#pragma once

#include <cassert>

namespace JkEng::Graphics
{
    template<typename T>
    class AfterBuildPtr
    {
    public:
        inline void Initialize(T* ptr)
        {
            assert(_ptr == nullptr);
            _ptr = ptr;
        }

        inline T* Get() const
        {
            assert(_ptr != nullptr);
            return _ptr;
        }

        inline T& operator*() const
        {
            return *(Get());
        }

        inline T* operator->() const
        {
            return Get();
        }

    private:
        T* _ptr = nullptr;
    };
}
