#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <JkEng/Graphics/OpenGL/OpenGLWrapper.h>
#include <JkEng/Graphics/OpenGL/Shader.h>
#include <JkEng/Graphics/OpenGL/ShaderProgram.h>
#include <JkEng/Graphics/OpenGL/VertexArray.h>
#include <JkEng/Graphics/OpenGL/ViewportCapture.h>
#include <jkengine/Window/GlfwWindow.h>
#include <jkengine/Window/GlfwWrapper.h>

using namespace testing;
using namespace JkEng::Graphics::OpenGL;

namespace
{
    struct TestSimpleVertex
    {
        float x;
        float y;
        float z;
    };

    const char *vertexShaderSource = R"GLSL(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
    )GLSL";

    const char *fragmentShaderSource = R"GLSL(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    )GLSL";

    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 600;
}

class VertexArrayTests : public Test
{
public:
    VertexArrayTests()
        : _glfw(),
          _window(_glfw, WINDOW_WIDTH, WINDOW_HEIGHT, "DummyIntegrationTestWindow"),
          _gl(_window),
          _shapeColor(255, 0, 0, 255), // matches hardcoded color in fragment shader
          _blackColor(0, 0, 0, 255)
    {
        // clear errors
        _gl.GetError();

        _testTriangleVertices.push_back(TestSimpleVertex({ 0.0f,  0.5f, 0.0f}));
        _testTriangleVertices.push_back(TestSimpleVertex({-0.5f, -0.5f, 0.0f}));
        _testTriangleVertices.push_back(TestSimpleVertex({ 0.5f, -0.5f, 0.0f}));

        _testRectangleVertices.push_back(TestSimpleVertex({-0.5f, -0.5f, 0.0f}));
        _testRectangleVertices.push_back(TestSimpleVertex({-0.5f,  0.5f, 0.0f}));
        _testRectangleVertices.push_back(TestSimpleVertex({ 0.5f,  0.5f, 0.0f}));
        _testRectangleVertices.push_back(TestSimpleVertex({ 0.5f, -0.5f, 0.0f}));

        _testTriangleElementsOfRectangle.push_back(0);
        _testTriangleElementsOfRectangle.push_back(1);
        _testTriangleElementsOfRectangle.push_back(2);
        _testTriangleElementsOfRectangle.push_back(2);
        _testTriangleElementsOfRectangle.push_back(3);
        _testTriangleElementsOfRectangle.push_back(0);

        Shader vertexShader(_gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(_gl, Shader::Type::Fragment, fragmentShaderSource);
        _shaderProgram.reset(new ShaderProgram(_gl, vertexShader, fragmentShader));
    }

protected:
    Window::GlfwWrapper _glfw;
    Window::GlfwWindow _window;
    OpenGLWrapper _gl;

    std::unique_ptr<ShaderProgram> _shaderProgram;

    std::vector<TestSimpleVertex> _testTriangleVertices;
    std::vector<TestSimpleVertex> _testRectangleVertices;
    std::vector<GLuint> _testTriangleElementsOfRectangle;

    const JkEng::Graphics::Color _shapeColor;
    const JkEng::Graphics::Color _blackColor;

    void ClearViewportToBlack()
    {
        _gl.ClearColor(_blackColor.r, _blackColor.g, _blackColor.b, _blackColor.a);
        _gl.Clear(GL_COLOR_BUFFER_BIT);
    }
};

TEST_F(VertexArrayTests, CreateAndDrawVertexArray_DoesNotThrow)
{
    // This test just simply makes sure the code does not generate errors
    // when interacting with the real library.  Higher-level tests
    // will be written elsewhere to draw a shape with real shaders and
    // assert that they are drawn correctly by reading a sampling of pixels.
    EXPECT_NO_THROW(
        VertexArray<TestSimpleVertex> vertexArray(
            _gl,
            VertexArray<TestSimpleVertex>::Params(_testRectangleVertices)
            .AddAttribute(3)
            .TriangleElementIndices(_testTriangleElementsOfRectangle)
        );

        vertexArray.Draw();
    );
}

TEST_F(VertexArrayTests, DrawRectangle_ReadAndVerifyPixels)
{
    VertexArray<TestSimpleVertex> vertexArray(
        _gl,
        VertexArray<TestSimpleVertex>::Params(_testRectangleVertices)
        .AddAttribute(3)
        .TriangleElementIndices(_testTriangleElementsOfRectangle)
    );

    ClearViewportToBlack();
    _shaderProgram->Use();
    vertexArray.Draw();
    _window.Update();

    ViewportCapture capture(_gl);

    // The 4 corners of the viewport should be the clearColor
    EXPECT_EQ(capture.GetPixel(0, 0), _blackColor);
    EXPECT_EQ(capture.GetPixel(0, capture.Height() - 1), _blackColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, capture.Height() - 1), _blackColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, 0), _blackColor);

    // The 4 corners of the rectangle should be _shapeColor
    // (check pixels 2 pixels in from the edge of the rectangle in each dimension
    // to allow for error in coordinate system conversions)

    // bottom left
    EXPECT_EQ(capture.GetPixel(capture.Width() / 4 + 2, capture.Height() / 4 + 2), _shapeColor);

    // top left
    EXPECT_EQ(capture.GetPixel(capture.Width() / 4 + 2, 3 * capture.Height() / 4 - 2), _shapeColor);

    // rop right
    EXPECT_EQ(capture.GetPixel(3 * capture.Width() / 4 - 2, 3 * capture.Height() / 4 - 2), _shapeColor);

    // bottom right
    EXPECT_EQ(capture.GetPixel(3 * capture.Width() / 4 - 2, capture.Height() / 4 + 2), _shapeColor);

    // Center should be _shapeColor
    EXPECT_EQ(capture.GetPixel(capture.Width() / 2, capture.Height() / 2), _shapeColor);
}

TEST_F(VertexArrayTests, DrawTriangle_ReadAndVerifyPixels)
{
    VertexArray<TestSimpleVertex> vertexArray(
        _gl,
        VertexArray<TestSimpleVertex>::Params(_testTriangleVertices)
        .AddAttribute(3)
    );

    ClearViewportToBlack();
    _shaderProgram->Use();
    vertexArray.Draw();
    _window.Update();

    ViewportCapture capture(_gl);
    capture.SaveToFileAsRaw("DrawTriangle_ReadAndVerifyPixels.data");

    // The 4 corners of the viewport should be the black color used to clear the viewport
    EXPECT_EQ(capture.GetPixel(0, 0), _blackColor);
    EXPECT_EQ(capture.GetPixel(0, capture.Height() - 1), _blackColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, capture.Height() - 1), _blackColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, 0), _blackColor);

    // The 3 corners of the triangle should be _shapeColor
    // (check a few pixels in from each corner to allow for error in
    // coordinate system conversions)

    // top corner 
    EXPECT_EQ(capture.GetPixel(capture.Width() / 2, capture.Height() / 4 + 2), _shapeColor);

    // bottom left corner
    EXPECT_EQ(capture.GetPixel(capture.Width() / 4 + 5, 3 * capture.Height() / 4 - 2), _shapeColor);
    
    // bottom right corner
    EXPECT_EQ(capture.GetPixel(3 * capture.Width() / 4 - 5, 3 * capture.Height() / 4 - 2), _shapeColor);

    // center should be _shapeColor
    EXPECT_EQ(capture.GetPixel(capture.Width() / 2, capture.Height() / 2), _shapeColor);
}
