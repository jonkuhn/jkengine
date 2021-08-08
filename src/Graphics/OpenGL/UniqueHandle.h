#pragma once
#include <glad/glad.h>

#include "IOpenGLWrapper.h"

namespace Graphics::OpenGL
{
    template<typename TDeleter>
    class UniqueHandle
    {
    public:
        typedef void (*Deleter)(IOpenGLWrapper&, GLuint);

        UniqueHandle(IOpenGLWrapper& gl, GLuint handle, TDeleter deleter)
          : _gl(&gl), _handle(handle), _deleter(deleter)
        {

        }

        ~UniqueHandle()
        {
            reset();
        }

        UniqueHandle(const UniqueHandle<TDeleter>& other) = delete;
        UniqueHandle<TDeleter>& operator=(const UniqueHandle<TDeleter>& other) = delete;

        UniqueHandle(UniqueHandle<TDeleter>&& other)
        {
            _gl = other._gl;
            _handle = other._handle;
            other._handle = 0;
            _deleter = other._deleter;
        }

        UniqueHandle<TDeleter>& operator=(UniqueHandle<TDeleter>&& other)
        {
            if (this == &other)
            {
                return *this;
            }

            _gl = other._gl;
            reset(other._handle);
            other._handle = 0;
            _deleter = other._deleter;

            return *this;
        }

        GLuint get() const
        {
            return _handle;
        }

        void reset(GLuint h = 0)
        {
            if (_gl != nullptr && _handle != 0 && _deleter != nullptr)
            {
                _deleter(*_gl, _handle);
            }
            _handle = h;
        }

    private:
        IOpenGLWrapper* _gl;
        GLuint _handle;
        TDeleter _deleter;
    };
}
