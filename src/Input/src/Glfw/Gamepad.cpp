#include "Glfw/Gamepad.h"

#include <sstream>

using namespace JkEng::Input::Glfw;

Gamepad::Gamepad(Window::IGlfwInputWrapper& glfw, int joystickId)
  : _glfw(&glfw),
    _joystickId(joystickId),
    _isConnected(false),
    _gamepadState()
{

}

const std::string Gamepad::DisplayName()
{
    auto* name = _glfw->GetJoystickName(_joystickId);
    if (name == nullptr)
    {
        return std::string();
    }

    std::stringstream ss;
    ss << "(" << std::to_string(_joystickId) << ") " << name;
    return ss.str();
}

bool Gamepad::IsConnected()
{
    return _isConnected;
}

bool Gamepad::RightFaceButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS);
}

bool Gamepad::LeftFaceButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS);
}

bool Gamepad::TopFaceButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS);
}

bool Gamepad::BottomFaceButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS);
}


bool Gamepad::DPadRight()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS);
}

bool Gamepad::DPadLeft()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS);
}

bool Gamepad::DPadUp()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS);
}

bool Gamepad::DPadDown()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS);
}


bool Gamepad::LeftStickButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS);
}

bool Gamepad::RightStickButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS);
}


bool Gamepad::LeftTriggerButton()
{
    // Glfw supports analog triggers and -1.0f is not pressed 1.0f is fully pressed
    return (_gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0.0f);
}

bool Gamepad::RightTriggerButton()
{
    // Glfw supports analog triggers and -1.0f is not pressed 1.0f is fully pressed
    return (_gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.0f);
}


bool Gamepad::LeftBumperButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS);
}

bool Gamepad::RightBumperButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS);
}


bool Gamepad::SelectButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS);
}

bool Gamepad::StartButton()
{
    return (_gamepadState.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS);
}


float Gamepad::LeftStickX()
{
    return _gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
}

float Gamepad::LeftStickY()
{
    return _gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
}


float Gamepad::RightStickX()
{
    return _gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
}

float Gamepad::RightStickY()
{
    return _gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
}

void Gamepad::Update()
{
    if (_glfw->GetGamepadState(_joystickId, &_gamepadState) == GLFW_FALSE)
    {
        _isConnected = false;
        return;
    }

    _isConnected = true;
}
