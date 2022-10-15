#pragma once

namespace Window
{
    // Marker interface for a GLFW window that is processing input events
    // This exists to document JkEng::Input::Glfw::Engine's dependence on a window
    // that processes input events
    class IGlfwInputWindow
    {
    public:

    protected:
        // This interface is not intended to expose ownership of the
        // concrete object implementing it.  Therefore, do not allow
        // deletion via interface pointers.
        ~IGlfwInputWindow() = default;
    };
}
