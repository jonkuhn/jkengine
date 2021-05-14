#pragma once

namespace Graphics::OpenGL
{
    // Marker interface for a window for OpenGL to draw in.
    // This exists to document OpenGLWrapper's dependence on a window to draw in.
    class IOpenGLWindow
    {

    protected:
        // This interface is not intended to expose ownership of the
        // concrete object implementing it.  Therefore, do not allow
        // deletion via interface pointers.
        ~IOpenGLWindow() = default;
    };
}
