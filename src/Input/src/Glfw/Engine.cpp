#include <stdexcept>

#include <Glfw/Engine.h>
#include <Glfw/Gamepad.h>

using namespace JkEng::Input::Glfw;

namespace
{
    template<int GamepadCount>
    std::array<Gamepad, GamepadCount> CreateGamepads(Window::IGlfwInputWrapper& glfw)
    {
        return std::array<Gamepad, GamepadCount>({{
            Gamepad(glfw, GLFW_JOYSTICK_1),
            Gamepad(glfw, GLFW_JOYSTICK_2),
            Gamepad(glfw, GLFW_JOYSTICK_3),
            Gamepad(glfw, GLFW_JOYSTICK_4),
            Gamepad(glfw, GLFW_JOYSTICK_5),
            Gamepad(glfw, GLFW_JOYSTICK_6),
            Gamepad(glfw, GLFW_JOYSTICK_7),
            Gamepad(glfw, GLFW_JOYSTICK_8),
            Gamepad(glfw, GLFW_JOYSTICK_9),
            Gamepad(glfw, GLFW_JOYSTICK_10),
            Gamepad(glfw, GLFW_JOYSTICK_11),
            Gamepad(glfw, GLFW_JOYSTICK_12),
            Gamepad(glfw, GLFW_JOYSTICK_13),
            Gamepad(glfw, GLFW_JOYSTICK_14),
            Gamepad(glfw, GLFW_JOYSTICK_15),
            Gamepad(glfw, GLFW_JOYSTICK_16)
        }});
    }
}

Engine::Engine(Window::IGlfwInputWrapper& glfw, Window::IGlfwInputWindow& inputWindow)
  : _glfw(glfw),
    _inputWindow(inputWindow),
    _gamepads(CreateGamepads<MaxGamepadCount>(_glfw)),
    _updateCalledAtLeastOnce(false)
{

}

void Engine::Update()
{
    // This is just used to support some sanity checks.
    // See EnumerateGamepads for details.
    if(!_updateCalledAtLeastOnce)
    {
        _updateCalledAtLeastOnce = true;
    }

    for (auto& gamepad : _gamepads)
    {
        gamepad.Update();
    }
}

std::vector<JkEng::Input::IGamepad*> Engine::EnumerateGamepads()
{
    // The results of this method are only up to date as of the
    // last time Update was called.  Update should be called
    // each frame in the main loop, but this quick check will
    // catch programming errors where EnumerateGamepads is called
    // before update is ever called.  This is to help catch mistakes
    // where Update is never called.
    if (!_updateCalledAtLeastOnce)
    {
        throw std::logic_error("Update must be called before EnumerateGamepads");
    }

    std::vector<IGamepad*> gamepads;

    for (auto& gamepad : _gamepads)
    {
        if (gamepad.IsConnected())
        {
            gamepads.push_back(&gamepad);
        }
    }

    return gamepads;
}