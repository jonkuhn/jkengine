#pragma once
#include <gmock/gmock.h>

#include <JkEng/Window/IGlfwInputWrapper.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockGlfwInputWrapper : public JkEng::Window::IGlfwInputWrapper {
public:
    MOCK_METHOD(const char*, GetJoystickName, (int joystickId), (override));
    MOCK_METHOD(int, GetGamepadState, (int joystickId, GLFWgamepadstate* state), (override));
};
#pragma clang diagnostic pop
