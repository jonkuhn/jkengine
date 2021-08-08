#include <string>
#include <sstream>

#include <gtest/gtest.h>


#include "Graphics/OpenGL/ShaderProgram.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/GlfwWrapper.h"
#include "Graphics/OpenGL/Window.h"
#include "Graphics/OpenGL/OpenGLWrapper.h"
#include "TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;

class ShaderProgramTests : public Test
{
public:
    ShaderProgramTests()
        : _glfw(),
          _window(&_glfw, 800, 600, "DummyIntegrationTestWindow"),
          _gl(&_window)
    {
        // clear errors
        _gl.GetError();
    }
protected:
    GlfwWrapper _glfw;
    GlfwWindow _window;
    OpenGLWrapper _gl;
};

namespace
{
    void AssertNoOpenGLErrors(IOpenGLWrapper& gl)
    {
        EXPECT_EQ(gl.GetError(), static_cast<unsigned int>(GL_NO_ERROR));
    }
}

TEST_F(ShaderProgramTests, CreateShaderProgramSuccess)
{
    OpenGLWrapper gl(&_window);

    Shader vertexShader(gl, Shader::Type::Vertex, GetValidVertexShaderCode());
    Shader fragmentShader(gl, Shader::Type::Fragment, GetValidFragmentShaderCode());
    EXPECT_NO_THROW(
        ShaderProgram shaderProgram(gl, {&vertexShader, &fragmentShader});
        shaderProgram.Use();
    );
}

TEST_F(ShaderProgramTests, SetVertexShaderUniforms)
{
    OpenGLWrapper gl(&_window);

    Shader vertexShader(gl, Shader::Type::Vertex, GetVertexShaderCodeWithUniforms());
    Shader fragmentShader(gl, Shader::Type::Fragment, GetValidFragmentShaderCode());
    ShaderProgram shaderProgram(gl, {&vertexShader, &fragmentShader});

    AssertNoOpenGLErrors(gl);

    shaderProgram.Use();
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testMatrix4", glm::mat4());
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testSampler", 123);
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testVector3", glm::vec3());
    AssertNoOpenGLErrors(gl);
}

TEST_F(ShaderProgramTests, SetFragmentShaderUniforms)
{
    OpenGLWrapper gl(&_window);

    Shader vertexShader(gl, Shader::Type::Vertex, GetValidVertexShaderCode());
    Shader fragmentShader(gl, Shader::Type::Fragment, GetFragmentShaderCodeWithUniforms());
    ShaderProgram shaderProgram(gl, {&vertexShader, &fragmentShader});

    AssertNoOpenGLErrors(gl);

    shaderProgram.Use();
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testMatrix4", glm::mat4());
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testSampler", 123);
    AssertNoOpenGLErrors(gl);
    shaderProgram.SetUniform("testVector3", glm::vec3());
    AssertNoOpenGLErrors(gl);
}

// In the future maybe write a test that intentionally
// produces a linking error if I figure out a reliable
// way to get compilation to succeed but linking to fail.
