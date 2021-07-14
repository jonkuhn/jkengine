#include <array>
#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include <gtest/gtest.h>

#include "Graphics/Graphics.h"

using namespace testing;
using namespace Graphics;

namespace
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);


    class TestTileMapImage : public IImage
    {
    private:
        static constexpr int WIDTH = 8;
        static constexpr int HEIGHT = 4;
        typedef std::array<std::array<std::array<uint8_t, 4>, WIDTH>, HEIGHT> PixelsType;

        // 8x4x4 array
        PixelsType _pixels;

    public:
        static constexpr std::array<uint8_t, 4> TileWhite = { 0, 0, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileRed   = { 1, 0, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileGreen = { 2, 0, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileBlue  = { 3, 0, 0, 0 };

        static constexpr std::array<uint8_t, 4> TileBlack   = { 0, 1, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileYellow  = { 1, 1, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileCyan    = { 2, 1, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileMagenta = { 3, 1, 0, 0 };

        static constexpr std::array<uint8_t, 4> TileDarkGray    = { 0, 2, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileLightPurple = { 1, 2, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileLightGreen  = { 2, 2, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileLightPeach  = { 3, 2, 0, 0 };

        static constexpr std::array<uint8_t, 4> TileLightGray = { 0, 3, 0, 0 };
        static constexpr std::array<uint8_t, 4> TilePurple    = { 1, 3, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileSkyBlue   = { 2, 3, 0, 0 };
        static constexpr std::array<uint8_t, 4> TileOrange    = { 3, 3, 0, 0 };

        TestTileMapImage()
            : _pixels({ {
                { TileYellow, TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue },
                { TileOrange, TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray },
                { TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray, TileOrange },
                { TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue, TileYellow }
            } })
        {

        }

        const uint8_t* Data() const override
        {
            return &_pixels[0][0][0];
        }
        int Width() const override
        {
            return WIDTH;
        }

        int Height() const override
        {
            return HEIGHT;
        }

        PixelFormat Format() const override
        {
            return PixelFormat::RGBA;
        }

    };
}

class TileMapTests : public Test
{
public:
    TileMapTests()
        : _engine(new OpenGL::Engine(WINDOW_WIDTH, WINDOW_HEIGHT, "TileMapTests"))
    {

    }
protected:
    std::unique_ptr<IEngine> _engine;
};

TEST_F(TileMapTests, GivenSolidColored4x4TileAtlas_AssignedInDifferentOrderIn16x16TileMap_ColorSamplesMatchExpectations)
{
    LibPngWrapper libPng;
    PngImage colorTiles4x4(&libPng, "TestFiles/colortiles4x4.png");
    auto atlas = _engine->CreateTileAtlas(colorTiles4x4, glm::vec2(4.0f, 4.0f));
    TestTileMapImage testTileMapImage;
    auto tileMap = atlas->CreateTileMap(testTileMapImage);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(static_cast<float>(testTileMapImage.Width()), static_cast<float>(testTileMapImage.Height()), 0.0f));  

    float cameraX = 4.0f;
    float cameraY = 2.0f;
    glm::vec3 cameraPos   = glm::vec3(cameraX, cameraY, 0.5f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    auto projection = glm::ortho(
        -4.0f,
        4.0f,
        -4.0f * (1 / ASPECT_RATIO),
        4.0f * (1 / ASPECT_RATIO),
        -1.0f,
        1.0f);


    auto& window = _engine->GetWindow();
    while (window.Update())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        tileMap->Draw(model, view, projection);
    }
}
