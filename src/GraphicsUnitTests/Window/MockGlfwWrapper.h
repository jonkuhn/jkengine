#pragma once
#include <gmock/gmock.h>

#include <jkengine/Window/IGlfwWrapper.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockGlfwWrapper : public Window::IGlfwWrapper {
public:
    MOCK_METHOD(void, WindowHint, (int hint, int value), (override));
    MOCK_METHOD(GLFWwindow*, CreateWindow, (int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share), (override));
    MOCK_METHOD(void, DestroyWindow, (GLFWwindow* window), (override));
    MOCK_METHOD(void, MakeContextCurrent, (GLFWwindow* window), (override));
    MOCK_METHOD(GLFWwindow*, GetCurrentContext, (), (override));
    MOCK_METHOD(GLFWframebuffersizefun, SetFramebufferSizeCallback, (GLFWwindow* window, GLFWframebuffersizefun callback), (override));
    MOCK_METHOD(int, GetError, (const char** description), (override));
    MOCK_METHOD(void, SwapBuffers, (GLFWwindow* window), (override));
    MOCK_METHOD(void, PollEvents, (), (override));
    MOCK_METHOD(int, GetKey, (GLFWwindow* window, int key), (override));
    MOCK_METHOD(void, SetWindowShouldClose, (GLFWwindow* window, int value), (override));
    MOCK_METHOD(int, WindowShouldClose, (GLFWwindow* window), (override));
    MOCK_METHOD(int, LoadGl, (), (override));
    MOCK_METHOD(void, SetGlViewport, (GLint x, GLint y, GLsizei width, GLsizei height), (override));
};
#pragma clang diagnostic pop
