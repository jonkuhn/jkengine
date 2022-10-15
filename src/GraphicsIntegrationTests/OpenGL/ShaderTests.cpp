#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <JkEng/Graphics/OpenGL/Shader.h>
#include <JkEng/Graphics/OpenGL/OpenGLWrapper.h>
#include <jkengine/Window/GlfwWindow.h>
#include <jkengine/Window/GlfwWrapper.h>

#include "TestHelpers.h"

using namespace testing;
using namespace JkEng::Graphics::OpenGL;

class ShaderTests : public Test
{
public:
    ShaderTests()
        : _glfw(),
          _window(_glfw, 800, 600, "DummyIntegrationTestWindow"),
          _gl(_window)
    {

    }
protected:
    Window::GlfwWrapper _glfw;
    Window::GlfwWindow _window;
    OpenGLWrapper _gl;
};

TEST_F(ShaderTests, CreateVertexShaderSuccess)
{
    EXPECT_NO_THROW(
        Shader shader(
            _gl,
            Shader::Type::Vertex,
            GetValidVertexShaderCode());
        EXPECT_NE(shader.Handle(), (GLuint)0);
    );
}

TEST_F(ShaderTests, CreateFragmentShaderSuccess)
{
    EXPECT_NO_THROW(
        Shader shader(
            _gl,
            Shader::Type::Fragment,
            GetValidFragmentShaderCode());
        EXPECT_NE(shader.Handle(), (GLuint)0);
    );
}

TEST_F(ShaderTests, CreateVertexShaderThrows)
{
    EXPECT_THROW(
        Shader shader(
            _gl,
            Shader::Type::Vertex,
            "this is not valid code"),
        std::runtime_error
    );
}

TEST_F(ShaderTests, CreateFragmentShaderThrows)
{
    EXPECT_THROW(
        Shader shader(
            _gl,
            Shader::Type::Fragment,
            "this is not valid code"),
        std::runtime_error
    );
}
