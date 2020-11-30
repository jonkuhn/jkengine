#include <stdexcept>

#include "GlfwWrapper.h"

using namespace Graphics::OpenGL;

class GlfwWrapper::GlfwInit
{
public:
    GlfwInit()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }
    }
    ~GlfwInit()
    {
        glfwTerminate();
    }
};

std::unique_ptr<GlfwWrapper::GlfwInit> GlfwWrapper::s_glfwInit;

GlfwWrapper::GlfwWrapper()
{
    if (!s_glfwInit)
    {
        s_glfwInit = std::make_unique<GlfwInit>();
    }
}
