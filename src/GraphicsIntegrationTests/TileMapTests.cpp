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
    static Color ColorBackgroundUglyYellow(0x80, 0x80, 0x00, 0xff);

    // Colors used in "colortiles4x4.png"
    static Color ColorWhite(0xff, 0xff, 0xff, 0xff);
    static Color ColorRed(0xff, 0x00, 0x00, 0xff);
    static Color ColorGreen(0x00, 0xff, 0x00, 0xff);
    static Color ColorBlue(0x00, 0x00, 0xff, 0xff);

    static Color ColorBlack(0x00, 0x00, 0x00, 0xff);
    static Color ColorYellow(0xff, 0xff, 0x00, 0xff);
    static Color ColorCyan(0x00, 0xff, 0xff, 0xff);
    static Color ColorMagenta(0xff, 0x00, 0xff, 0xff);

    static Color ColorDarkGray(0x44, 0x44, 0x44, 0xff);
    static Color ColorLightPurple(0xcc, 0xcc, 0xff, 0xff);
    static Color ColorLightGreen(0xcc, 0xff, 0xcc, 0xff);
    static Color ColorLightPeach(0xff, 0xcc, 0xcc, 0xff);

    static Color ColorLightGray(0xcc, 0xcc, 0xcc, 0xff);
    static Color ColorPurple(0xaa, 0x00, 0xff, 0xff);
    static Color ColorSkyBlue(0x00, 0xaa, 0xff, 0xff);
    static Color ColorOrange(0xff, 0xaa, 0x00, 0xff);

    // Positions of colors used in "colortiles4x4.png"
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


    class TestTileMapImage : public IImage
    {
    private:
        static constexpr int WIDTH = 8;
        static constexpr int HEIGHT = 4;
        typedef std::array<std::array<std::array<uint8_t, 4>, WIDTH>, HEIGHT> TileIndicesType;
        typedef std::array<std::array<Color, WIDTH>, HEIGHT> TileColorsType;

        // 8x4x4 array
        TileIndicesType _tiles;

    public:
        TileColorsType TileColors;

        TestTileMapImage()
            : _tiles({ {
                { TileYellow, TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue },
                { TileOrange, TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray },
                { TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray, TileOrange },
                { TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue, TileYellow }
            } }),
              TileColors({ {
                { ColorYellow, ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue },
                { ColorOrange, ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray },
                { ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray, ColorOrange },
                { ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue, ColorYellow }
            } })
        {

        }

        const uint8_t* Data() const override
        {
            return &_tiles[0][0][0];
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

    template<unsigned int ColumnCount, unsigned int RowCount>
    void ExpectTileColorsInScreenshot(
        IScreenshot& screenshot,
        std::array<std::array<Color, ColumnCount>, RowCount> expectedTileColors)
    {
        auto columnWidth = screenshot.Width() / ColumnCount;
        auto rowHeight = screenshot.Height() / RowCount;
        auto sampleDistanceFromEdgeX = columnWidth / 16;
        auto sampleDistanceFromEdgeY = rowHeight / 16;

        // sample each tile near 4 corners plus center
        auto sampleLeftX = sampleDistanceFromEdgeX;
        auto sampleUpperY = sampleDistanceFromEdgeY;
        auto sampleRightX = columnWidth - sampleDistanceFromEdgeX;
        auto sampleLowerY = rowHeight - sampleDistanceFromEdgeY;
        auto sampleCenterX = columnWidth / 2;
        auto sampleCenterY = rowHeight / 2;
        
        for(unsigned int row = 0; row < RowCount; row++)
        {
            for(unsigned int column = 0; column < ColumnCount; column++)
            {
                auto expectedColor = expectedTileColors[row][column];
                auto x = column * columnWidth;
                auto y = row * rowHeight;
                EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleLeftX, y + sampleUpperY));
                EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleRightX, y + sampleUpperY));
                EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleCenterX, y + sampleCenterY));
                EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleLeftX, y + sampleLowerY));
                EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleRightX, y + sampleLowerY));
            }
        }
    }
}

class TileMapTests : public Test
{
public:
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 600;
    static constexpr float AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

    TileMapTests()
    {

    }
};

TEST_F(TileMapTests, GivenSolidColored4x4TileAtlas_AssignedInDifferentOrderIn16x16TileMap_ColorSamplesMatchExpectations)
{
    auto engine = std::make_unique<OpenGL::Engine>(
       WindowWidth, WindowHeight, "TileMapTests");

    LibPngWrapper libPng;
    PngImage colorTiles4x4(&libPng, "TestFiles/colortiles4x4.png");
    auto atlas = engine->CreateTileAtlas(colorTiles4x4, glm::vec2(4.0f, 4.0f));
    TestTileMapImage testTileMapImage;
    auto tileMap = atlas->CreateTileMap(testTileMapImage);
    
    auto tileMapInstance = tileMap->CreateInstance();
    tileMapInstance->Position(glm::vec3(0.0f, 0.0f, 0.0f));
    tileMapInstance->Rotation(0.0f);
    tileMapInstance->Size(glm::vec2(static_cast<float>(testTileMapImage.Width()), static_cast<float>(testTileMapImage.Height())));

    auto camera = engine->Camera2d();
    camera->Center(glm::vec2(4.0f, 2.0f));
    camera->FieldOfView(ICamera2d::Fov(
        -4.0f,
        4.0f,
        -4.0f * (1 / AspectRatio),
        4.0f * (1 / AspectRatio),
        1.0f,
        -1.0f));

    while(!engine->ProgramShouldExit())
    {
        glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        tileMap->DrawAllInstances();
        engine->Render();
    }

    auto scr = engine->TakeScreenshot();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorsInScreenshot<ColumnCount, RowCount>(
        *scr,
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // First row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Middle 4 rows should be equal to the colors of the tiles in the tile atlas
            // as selected by TestTileMapImage
            testTileMapImage.TileColors[0],
            testTileMapImage.TileColors[1],
            testTileMapImage.TileColors[2],
            testTileMapImage.TileColors[3],

            // Last row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } }));
}
