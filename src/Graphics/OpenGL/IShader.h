#pragma once
#include <glad/glad.h>

namespace Graphics::OpenGL
{
    class IShader
    {
    public:
        virtual GLuint Handle() const = 0;

    protected:
        // This interface is not intended to expose ownership of the
        // concrete object implementing it.  Therefore, do not allow
        // deletion via interface pointers.
        ~IShader() = default;
    };
}
