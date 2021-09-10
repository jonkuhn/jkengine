#pragma once
#include <GLFW/glfw3.h>

namespace Window
{
    class IGlfwInputWrapper
    {
    public:
        virtual const char* GetJoystickName(int joystickId) = 0;
        virtual int GetGamepadState(int joystickId, GLFWgamepadstate* state) = 0;


    protected:
        // This interface is not intended to expose ownership of the
        // concrete object implementing it.  Therefore, do not allow
        // deletion via interface pointers.
        ~IGlfwInputWrapper() = default;
    };
}
