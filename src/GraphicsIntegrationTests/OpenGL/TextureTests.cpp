#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Graphics/PngImage.h"
#include "Graphics/LibPngWrapper.h"

#include "Graphics/OpenGL/GlfwWrapper.h"
#include "Graphics/OpenGL/Window.h"
#include "Graphics/OpenGL/OpenGLWrapper.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/ShaderProgram.h"
#include "Graphics/OpenGL/Texture.h"
#include "Graphics/OpenGL/VertexArray.h"
#include "Graphics/OpenGL/ViewportCapture.h"

#include "TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;
using LibPngWrapper = Graphics::LibPngWrapper;
using PngImage = Graphics::PngImage;

namespace
{
    struct TestTexturedVertex
    {
        float x;
        float y;
        float z;
        float s; // s = texture x coordinate
        float t; // t = texture y coordinate
    };

    const char *vertexShaderSource = R"GLSL(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    
    out vec3 ourColor;
    out vec2 TexCoord;
    
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
    )GLSL";

    const char *fragmentShaderSource = R"GLSL(
    #version 330 core
    out vec4 FragColor;
      
    in vec2 TexCoord;
    
    uniform sampler2D ourTexture;
    
    void main()
    {
        vec4 texColor = texture(ourTexture, TexCoord);

        // discard fragments with low alpha values
        // (currently these test do not enable alpha blending)
        if (texColor.a < 0.1)
        {
            discard;
        }
        FragColor = texColor;
    }
    )GLSL";

    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 600;
}

class TextureTests : public Test
{
public:
    TextureTests()
        : _libpng(),
          _glfw(),
          _window(&_glfw, WINDOW_WIDTH, WINDOW_HEIGHT, "DummyIntegrationTestWindow"),
          _gl(&_window),
          _blackColor(0, 0, 0, 255),
          _redColor(255, 0, 0, 255),
          _greenColor(0, 255, 0, 255),
          _blueColor(0, 0, 255, 255),
          _whiteColor(255, 255, 255, 255),
          _pinkColor(255, 0, 255, 255)
    {
        // clear errors
        _gl.GetError();

        // Note the way the t texture coordinates are specified causes OpenGL to
        // render the image the way it normally displays rather than being inverted
        _testRectangleVertices.push_back(TestTexturedVertex({-0.5f, -0.5f, 0.0f, 0.0f, 1.0f})); // bottom left
        _testRectangleVertices.push_back(TestTexturedVertex({-0.5f,  0.5f, 0.0f, 0.0f, 0.0f})); // top left
        _testRectangleVertices.push_back(TestTexturedVertex({ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f})); // top right
        _testRectangleVertices.push_back(TestTexturedVertex({ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f})); // bottom right

        _testTriangleElementsOfRectangle.push_back(0);
        _testTriangleElementsOfRectangle.push_back(1);
        _testTriangleElementsOfRectangle.push_back(2);
        _testTriangleElementsOfRectangle.push_back(2);
        _testTriangleElementsOfRectangle.push_back(3);
        _testTriangleElementsOfRectangle.push_back(0);

        Shader vertexShader(_gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(_gl, Shader::Type::Fragment, fragmentShaderSource);
        _shaderProgram.reset(new ShaderProgram(_gl, {&vertexShader, &fragmentShader}));
    }
protected:
    LibPngWrapper _libpng;
    GlfwWrapper _glfw;
    GlfwWindow _window;
    OpenGLWrapper _gl;

    std::unique_ptr<ShaderProgram> _shaderProgram;
    std::vector<TestTexturedVertex> _testRectangleVertices;
    std::vector<GLuint> _testTriangleElementsOfRectangle;

    const Graphics::Color _blackColor;
    const Graphics::Color _redColor;
    const Graphics::Color _greenColor;
    const Graphics::Color _blueColor;
    const Graphics::Color _whiteColor;
    const Graphics::Color _pinkColor;

    void ClearViewportToPink()
    {
        _gl.ClearColor(_pinkColor.r, _pinkColor.g, _pinkColor.b, _pinkColor.a);
        _gl.Clear(GL_COLOR_BUFFER_BIT);
    }
};

TEST_F(TextureTests, CreateBindAndDestroyTexture_DoesNotThrow)
{
    // This test just simply makes sure the code does not generate errors
    // when interacting with the real library.  Higher-level tests
    // will be written elsewhere to draw a textured shape with real shaders and
    // assert that they are drawn correctly by reading a sampling of pixels.
    EXPECT_NO_THROW(
        PngImage image(&_libpng, "TestFiles/test31x47.png");
        Texture texture(
            _gl,
            Texture::Params(image)
                .WrapModeS(Texture::WrapMode::Repeat)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::NearestMipmapNearest)
                .MagFilter(Texture::MagFilterMode::Linear));

        texture.Bind(0);
    );
}

TEST_F(TextureTests, DrawTexturedRectangle_ReadAndVerifyPixels)
{
    VertexArray<TestTexturedVertex> vertexArray(
        _gl,
        VertexArray<TestTexturedVertex>::Params(_testRectangleVertices)
        .AddAttribute(3) // position x, y, z
        .AddAttribute(2) // texture s and t
        .TriangleElementIndices(_testTriangleElementsOfRectangle)
    );
    PngImage image(&_libpng, "TestFiles/test31x47.png");
    Texture texture(
        _gl,
        Texture::Params(image)
            .WrapModeS(Texture::WrapMode::ClampToBorder)
            .WrapModeT(Texture::WrapMode::ClampToBorder)
            .MinFilter(Texture::MinFilterMode::Nearest)
            .MagFilter(Texture::MagFilterMode::Nearest));

    texture.Bind(0);

    ClearViewportToPink();
    _shaderProgram->Use();
    vertexArray.Draw();
    _window.Update();

    ViewportCapture capture(_gl);

    capture.SaveToFileAsRaw("testtexture.data");

    // The 4 corners of the viewport should be the clearColor (pink)
    EXPECT_EQ(capture.GetPixel(0, 0), _pinkColor);
    EXPECT_EQ(capture.GetPixel(0, capture.Height() - 1), _pinkColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, capture.Height() - 1), _pinkColor);
    EXPECT_EQ(capture.GetPixel(capture.Width() - 1, 0), _pinkColor);

    auto rectangleCaptureHeight = capture.Height() / 2;

    // The test31x47.png image consists of the following
    // (top to bottom as PNG is normally displayed -- which is
    // how it will be displayed on the screen.  Texture coordinates
    // take into account converting between conventional window/image
    // y-axis and OpenGL's.  Note that ViewportCapture::GetPixel works
    // in terms of conventional coordinates with y-axis going from top
    // to bottom)
    // 31 x 3 black rectangle
    // 31 x 3 red rectangle
    // 31 x 3 green rectangle
    // 31 x 3 blue rectangle
    // 31 x 3 transparent rectangle
    // 31 x 32 white rectangle

    constexpr int smallRectTextureHeight = 3;
    constexpr int whiteRectTextureHeight = 32;
    constexpr int totalTextureHeight = 47;

    const int smallRectCaptureHeight = rectangleCaptureHeight * smallRectTextureHeight / totalTextureHeight;

    // sample the image at 5 places along the horizontal
    auto sampleLeftBgX = capture.Width() / 4 - 5; // sample background to the left
    auto sampleLeftRectX = capture.Width() / 4 + 5; // sample left of rectangle
    auto sampleMiddleRectX = capture.Width() / 2; // sample middle of rectangle
    auto sampleRightRectX = 3 * capture.Width() / 4 - 5; // sample right of rectangle
    auto sampleRightBgX = 3 * capture.Width() / 4 + 5; // sample background to the right

    // do the first samples in the middle of the black rectangle
    auto sampleY = capture.Height() / 4 + smallRectCaptureHeight / 2;

    EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _blackColor);
    EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _blackColor);
    EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _blackColor);
    EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);

    // sample the next (red) rectangle
    sampleY += smallRectCaptureHeight;
    EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _redColor);
    EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _redColor);
    EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _redColor);
    EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);

    // sample the next (green) rectangle
    sampleY += smallRectCaptureHeight;
    EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _greenColor);
    EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _greenColor);
    EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _greenColor);
    EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);

    // sample the next (blue) rectangle
    sampleY += smallRectCaptureHeight;
    EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _blueColor);
    EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _blueColor);
    EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _blueColor);
    EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);

    // sample the next (transparent) rectangle (shows as pink)
    sampleY += smallRectCaptureHeight;
    EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _pinkColor);
    EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);

    // sample the big white rectangle
    constexpr int whiteRectSampleCount = whiteRectTextureHeight / smallRectTextureHeight;
    for (int i = 0; i < whiteRectSampleCount; i++)
    {
        sampleY += smallRectCaptureHeight;
        EXPECT_EQ(capture.GetPixel(sampleLeftBgX, sampleY), _pinkColor);
        EXPECT_EQ(capture.GetPixel(sampleLeftRectX, sampleY), _whiteColor);
        EXPECT_EQ(capture.GetPixel(sampleMiddleRectX, sampleY), _whiteColor);
        EXPECT_EQ(capture.GetPixel(sampleRightRectX, sampleY), _whiteColor);
        EXPECT_EQ(capture.GetPixel(sampleRightBgX, sampleY), _pinkColor);
    }
}
