#pragma once
#include <string>

#include "../IGamepad.h"
#include <jkengine/Window/IGlfwInputWrapper.h>

namespace Input::Glfw
{
    class Gamepad final : public IGamepad
    {
    public:
        Gamepad(Window::IGlfwInputWrapper& glfw, int joystickId);

        // There is need to copy this class.
        Gamepad(const Gamepad&) = delete;
        Gamepad& operator=(const Gamepad&) = delete;

        // This class must be move-able for initialization of std::array of Gamepad
        Gamepad(Gamepad&&) = default;
        Gamepad& operator=(Gamepad&&) = default;

        const std::string DisplayName() override;
        bool IsConnected() override;

        bool RightFaceButton() override;
        bool LeftFaceButton() override;
        bool TopFaceButton() override;
        bool BottomFaceButton() override;

        bool DPadRight() override;
        bool DPadLeft() override;
        bool DPadUp() override;
        bool DPadDown() override;

        bool LeftStickButton() override;
        bool RightStickButton() override;

        bool LeftTriggerButton() override;
        bool RightTriggerButton() override;

        bool LeftBumperButton() override;
        bool RightBumperButton() override;

        bool SelectButton() override;
        bool StartButton() override;

        float LeftStickX() override;
        float LeftStickY() override;

        float RightStickX() override;
        float RightStickY() override;

        // Methods only intended for use by Input::Glfw::Engine
        void Update();

    private:
        Window::IGlfwInputWrapper* _glfw;
        int _joystickId;
        bool _isConnected;
        GLFWgamepadstate _gamepadState;
    };
}
