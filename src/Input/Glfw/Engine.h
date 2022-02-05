#pragma once

#include <array>
#include <vector>

#include <Window/IGlfwInputWindow.h>
#include <Window/IGlfwInputWrapper.h>
#include "../IEngine.h"

#include "Gamepad.h"

namespace Input::Glfw
{
    class Engine : public IEngine
    {
    public:
        Engine(Window::IGlfwInputWrapper& glfw, Window::IGlfwInputWindow& inputWindow);

        // There is need to copy or move this class.
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

        void Update() override;
        std::vector<IGamepad*> EnumerateGamepads() override;
    
    private:
        static constexpr unsigned int MaxGamepadCount = GLFW_JOYSTICK_LAST + 1;

        Window::IGlfwInputWrapper& _glfw;

        // Unused, but exists to document dependence on a window that polls input events
        // (May not be true for gamepad input but as this class is expanded it will be true)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunused-private-field"
        Window::IGlfwInputWindow& _inputWindow;
        #pragma clang diagnostic pop

        std::array<Gamepad, MaxGamepadCount> _gamepads;
        bool _updateCalledAtLeastOnce;
    };
}
