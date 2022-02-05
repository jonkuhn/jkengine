#pragma once

#include <memory>

#include "IGlfwWrapper.h"
#include "IGlfwInputWrapper.h"

namespace Window
{
    class GlfwWrapper final : public IGlfwWrapper, public IGlfwInputWrapper
    {
    public:
        GlfwWrapper();
        GlfwWrapper(const GlfwWrapper&) = delete;
        GlfwWrapper& operator=(const GlfwWrapper&) = delete;
        GlfwWrapper(GlfwWrapper&&) = default;
        GlfwWrapper& operator=(GlfwWrapper&&) = default;

        void WindowHint(int hint, int value) override
        {
            glfwWindowHint(hint, value);
        }

        GLFWwindow* CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) override
        {
            return glfwCreateWindow(width, height, title, monitor, share);
        }

        void DestroyWindow(GLFWwindow* window) override
        {
            glfwDestroyWindow(window);
        }

        void MakeContextCurrent(GLFWwindow* window) override
        {
            glfwMakeContextCurrent(window);
        }

        GLFWwindow* GetCurrentContext() override
        {
            return glfwGetCurrentContext();
        }

        GLFWframebuffersizefun SetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback) override
        {
            return glfwSetFramebufferSizeCallback(window, callback);
        }

        int GetError(const char** description) override
        {
            return glfwGetError(description);
        }

        void SwapBuffers(GLFWwindow* window) override
        {
            glfwSwapBuffers(window);
        }

        void PollEvents() override
        {
            glfwPollEvents();
        }

        int GetKey(GLFWwindow* window, int key) override
        {
            return glfwGetKey(window, key);
        }

        void SetWindowShouldClose(GLFWwindow* window, int value) override
        {
            glfwSetWindowShouldClose(window, value);
        }

        int WindowShouldClose(GLFWwindow* window) override
        {
            return glfwWindowShouldClose(window);
        }

        int LoadGl() override
        {
            return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }

        void SetGlViewport(GLint x, GLint y, GLsizei width, GLsizei height) override
        {
            return glViewport(x, y, width, height);
        }

        const char* GetJoystickName(int joystickId) override
        {
            return glfwGetJoystickName(joystickId);
        }

        int GetGamepadState(int joystickId, GLFWgamepadstate* state) override
        {
            return glfwGetGamepadState(joystickId, state);
        }

    private:
        class GlfwInit;
        static std::unique_ptr<GlfwInit> s_glfwInit;
    };
}
