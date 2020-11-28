#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics::OpenGL
{
    class IGlfwWrapper
    {
    public:
        virtual int Init(void) = 0;
        virtual void Terminate(void) = 0;
        virtual void WindowHint(int hint, int value) = 0;
        virtual GLFWwindow* CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) = 0;
        virtual void MakeContextCurrent(GLFWwindow* window) = 0;
        virtual GLFWwindow* GetCurrentContext(void) = 0;
        virtual GLFWframebuffersizefun SetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback) = 0;
        virtual int GetError(const char** description) = 0;
        virtual int LoadGLLoader() = 0;
    };
}
