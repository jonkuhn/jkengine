#pragma once

#include "IGlfwWrapper.h"

namespace Graphics::OpenGL
{
    class GlfwWrapper : public IGlfwWrapper
    {
    public:
        int Init() override
        {
            return glfwInit();
        }
        void Terminate() override
        {
            glfwTerminate();
        }
        void WindowHint(int hint, int value) override
        {
            glfwWindowHint(hint, value);
        }
        GLFWwindow* CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) override
        {
            return glfwCreateWindow(width, height, title, monitor, share);
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
        int LoadGLLoader() override
        {
            return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }
    };
}