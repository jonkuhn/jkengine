#include <array>
#include <cmath>
#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include <gtest/gtest.h>

#include "Graphics/Graphics.h"

#include "ColorTiles4x4.h"
#include "ScreenSampleHelpers.h"

using namespace testing;
using namespace Graphics;

namespace
{
    class TileMapImage8x4 : public IImage
    {
    public:
        static constexpr int WIDTH = 8;
        static constexpr int HEIGHT = 4;
        typedef std::array<std::array<std::array<uint8_t, 4>, WIDTH>, HEIGHT> TileIndicesType;
        typedef std::array<std::array<Color, WIDTH>, HEIGHT> TileColorsType;

        TileMapImage8x4(TileIndicesType tileIndices, TileColorsType tileColors)
            : Tiles(tileIndices), TileColors(tileColors)
        {

        }

        const uint8_t* Data() const override
        {
            return &Tiles[0][0][0];
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

        // 8x4x4 array
        TileIndicesType Tiles;
        TileColorsType TileColors;
    };

}

class TileMapTests : public Test
{
public:
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 600;
    static constexpr float AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

    static constexpr unsigned int DrawingLayers = 3;

    TileMapTests()
      : _libPng(),
        _engine(std::make_unique<OpenGL::Engine>(WindowWidth, WindowHeight, "TileMapTests", DrawingLayers)),
        _camera(_engine->Camera2d()),
        _atlasColorTiles4x4(),
        _atlasColorTilesEmptyCenters4x4(),
        _tileMapImage8x4(
            TileMapImage8x4::TileIndicesType({ {
                { TileYellow, TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue },
                { TileOrange, TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray },
                { TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray, TileOrange },
                { TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue, TileYellow }
            } }),
            TileMapImage8x4::TileColorsType({ {
                { ColorYellow, ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue },
                { ColorOrange, ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray },
                { ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray, ColorOrange },
                { ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue, ColorYellow }
            } })
        ),
        _tileMapImage8x4FlippedTopToBottom(
            TileMapImage8x4::TileIndicesType({ {
                { TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue, TileYellow },
                { TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray, TileOrange },
                { TileOrange, TileWhite, TileBlue, TileLightGray, TileLightPeach, TileBlack, TileMagenta, TileDarkGray },
                { TileYellow, TileGreen, TileCyan, TileRed, TileLightGreen, TilePurple, TileLightPurple, TileSkyBlue }
            } }),
            TileMapImage8x4::TileColorsType({ {
                { ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue, ColorYellow },
                { ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray, ColorOrange },
                { ColorOrange, ColorWhite, ColorBlue, ColorLightGray, ColorLightPeach, ColorBlack, ColorMagenta, ColorDarkGray },
                { ColorYellow, ColorGreen, ColorCyan, ColorRed, ColorLightGreen, ColorPurple, ColorLightPurple, ColorSkyBlue }
            } })
        ),
        _colorTilesMap8x4(),
        _colorTilesMap8x4FlippedTopToBottom(),
        _colorTilesMapEmptyCenters8x4()
    {
        _engine->ClearColor(ColorBackgroundUglyYellow);
        PngImage colorTiles4x4(&_libPng, "TestFiles/colortiles4x4.png");
        _atlasColorTiles4x4 = _engine->CreateTileAtlas(colorTiles4x4, glm::vec2(4.0f, 4.0f));
        _colorTilesMap8x4 = _atlasColorTiles4x4->CreateTileMap(_tileMapImage8x4);
        _colorTilesMap8x4FlippedTopToBottom = _atlasColorTiles4x4->CreateTileMap(_tileMapImage8x4FlippedTopToBottom);

        PngImage colorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        _atlasColorTilesEmptyCenters4x4 = _engine->CreateTileAtlas(colorTiles4x4EmptyCenters, glm::vec2(4.0f, 4.0f));
        _colorTilesMapEmptyCenters8x4 = _atlasColorTilesEmptyCenters4x4->CreateTileMap(_tileMapImage8x4);
    }
protected:
    LibPngWrapper _libPng;
    std::unique_ptr<IEngine> _engine;
    ICamera2d* _camera;
    std::unique_ptr<ITileAtlas> _atlasColorTiles4x4;
    std::unique_ptr<ITileAtlas> _atlasColorTilesEmptyCenters4x4;
    TileMapImage8x4 _tileMapImage8x4;
    TileMapImage8x4 _tileMapImage8x4FlippedTopToBottom;
    std::unique_ptr<ITileMap> _colorTilesMap8x4;
    std::unique_ptr<ITileMap> _colorTilesMap8x4FlippedTopToBottom;
    std::unique_ptr<ITileMap> _colorTilesMapEmptyCenters8x4;
    
    std::unique_ptr<IObjectInstance2d> SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(ITileMap& tileMap, unsigned int layer)
    {
        // Create instance of 8x4 tile map with lower left at origin
        // with no rotation and size in world coordinates set to
        // the width and height of the tile map (8x4)
        auto tileMapInstance = tileMap.CreateInstance(layer);
        tileMapInstance->Position(glm::vec2(0.0f, 0.0f));
        tileMapInstance->Rotation(0.0f);
        tileMapInstance->Size(glm::vec2(
            static_cast<float>(_tileMapImage8x4.Width()),
            static_cast<float>(_tileMapImage8x4.Height())));

        // Center camera at the center of the tile map
        // (since lower left of tile map is at origin, taking half
        // of the width and height of the tile map will give the
        // center point)
        _camera->Center(tileMapInstance->Size() / 2.0f);

        // Set FoV so that 8 tiles are visible horizontally and due
        // to the aspect ration 6 tiles will be visible vertically
        _camera->FieldOfView(ICamera2d::Fov(
            -4.0f, 4.0f,
            -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

        return tileMapInstance;
    }

    template<unsigned int ColumnCount, unsigned int RowCount>
    void ExpectTileColorGridOnScreenWithOrientationBrownAndSeparateCenterColors(
        std::array<std::array<Color, ColumnCount>, RowCount> expectedTileColors,
        std::array<std::array<Color, ColumnCount>, RowCount> expectedCenterColors)
    {
        auto screenshot = _engine->TakeScreenshot();
        auto columnWidth = screenshot->Width() / ColumnCount;
        auto rowHeight = screenshot->Height() / RowCount;
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

                // Expect upper left to be orientation brown if it is "part
                // of a tile", but otherwise expect it to be the background
                // color.  If the expected color for the tile is not equal
                // to the background color it is considered to be "part of a tile"
                auto expectedUpperLeftColor = 
                    (expectedColor != ColorBackgroundUglyYellow)
                    ? ColorOrientationBrown
                    : ColorBackgroundUglyYellow;

                EXPECT_EQ(expectedUpperLeftColor, screenshot->GetPixel(x + sampleLeftX, y + sampleUpperY))
                    << "(upper left) row = " << row << " column = " << column;
                EXPECT_EQ(expectedColor, screenshot->GetPixel(x + sampleRightX, y + sampleUpperY))
                    << "(upper right) row = " << row << " column = " << column;

                auto expectedCenterColor = expectedCenterColors[row][column];
                EXPECT_EQ(expectedCenterColor, screenshot->GetPixel(x + sampleCenterX, y + sampleCenterY))
                    << "(center) row = " << row << " column = " << column;

                EXPECT_EQ(expectedColor, screenshot->GetPixel(x + sampleLeftX, y + sampleLowerY))
                    << "(lower left) row = " << row << " column = " << column;
                EXPECT_EQ(expectedColor, screenshot->GetPixel(x + sampleRightX, y + sampleLowerY))
                    << "(lower right) row = " << row << " column = " << column;
            }
        }
    }

    void ExpectColorAtScreenPositionIfOnScreen(
        IScreenshot& screenshot,
        std::string name,
        Color expectedColor,
        unsigned int x,
        unsigned int y,
        unsigned int row,
        unsigned int column)
    {
        // only check points that are on screen
        if(x > 0 && x < screenshot.Width() && y > 0 && y < screenshot.Height())
        {
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x, y))
                << "(" << name <<  ") row = " << row << " column = " << column;
        }
    }

    template<unsigned int ColumnCount, unsigned int RowCount>
    void ExpectRotatedTileColorGridOnScreen(
        std::array<std::array<Color, ColumnCount>, RowCount> expectedTileColors)
    {
        auto screenshot = _engine->TakeScreenshot();

        // Assume non-rotated tiles fit exactly 8 across for calculating
        // tileDiagonal
        auto tileDiagonal = static_cast<unsigned int>(
            sqrt(2.0f) * static_cast<float>(screenshot->Width()) / 8.0f);

        auto columnWidth = tileDiagonal;
        auto rowHeight = tileDiagonal / 2;

        auto firstCenterX = (screenshot->Width() / 2) - (5 * tileDiagonal / 2);
        auto firstCenterY = (screenshot->Height() / 2) - (2 * tileDiagonal);
        auto oddRowXOffset = tileDiagonal / 2;

        // sample each tile near 4 corners plus center
        auto sampleCenterXOffset = 0.0f;
        auto sampleCenterYOffset = 0.0f;

        auto sampleDistanceFromCorner = tileDiagonal / 16.0f;
        auto sampleLeftXOffset = -0.5 * tileDiagonal + sampleDistanceFromCorner;
        auto sampleRightXOffset = 0.5 * tileDiagonal - sampleDistanceFromCorner;
        auto sampleTopYOffset = -0.5 * tileDiagonal + sampleDistanceFromCorner;
        auto sampleBottomYOffset = 0.5 * tileDiagonal - sampleDistanceFromCorner;
        
        for(unsigned int row = 0; row < RowCount; row++)
        {
            for(unsigned int column = 0; column < ColumnCount; column++)
            {
                auto expectedColor = expectedTileColors[row][column];
                auto x = firstCenterX + column * columnWidth + ((row % 2) * oddRowXOffset);
                auto y = firstCenterY + row * rowHeight;

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "center", expectedColor,
                    x + sampleCenterXOffset, y + sampleCenterYOffset, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "left", expectedColor,
                    x + sampleLeftXOffset, y + sampleCenterYOffset, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "right", expectedColor,
                    x + sampleRightXOffset, y + sampleCenterYOffset, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "top", expectedColor,
                    x + sampleCenterXOffset, y + sampleTopYOffset, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "bottom", expectedColor,
                    x + sampleCenterXOffset, y + sampleBottomYOffset, row, column);
            }
        }
    }

    void Expect8x4ColorTilesMapCenteredOnScreen()
    {
        const unsigned int ColumnCount = 8;
        const unsigned int RowCount = 6;
        ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
            *(_engine->TakeScreenshot()),
            std::array<std::array<Color, ColumnCount>, RowCount>(
            { {
                // First row is background color
                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                // Middle 4 rows should be equal to the colors of the tiles in the tile atlas
                // as selected by TileMapImage8x4
                _tileMapImage8x4.TileColors[0],
                _tileMapImage8x4.TileColors[1],
                _tileMapImage8x4.TileColors[2],
                _tileMapImage8x4.TileColors[3],

                // Last row is background color
                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
            } })
        );
    }

    void ExpectAllBackgroundColorOnScreen()
    {
        const unsigned int ColumnCount = 8;
        const unsigned int RowCount = 6;
        ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
            *(_engine->TakeScreenshot()),
            std::array<std::array<Color, ColumnCount>, RowCount>(
            { {
                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

                { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
                ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
            } })
        );
    }
};

TEST_F(TileMapTests, Given8x4TileMapAtOrigin_Camera8x6FovCenteredAtCenterOfTileMap_EntireTileMapIsVisible)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4, 0);
    _engine->Render();
    Expect8x4ColorTilesMapCenteredOnScreen();
}


TEST_F(TileMapTests, Given8x4TileMapOffCamera_Camera8x6FovCenteredAtCenterOfTileMap_EntireAllBackgroundColorOnScreen)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4, 0);

    // Move tile map to an off camera location and expect all background color
    tileMapInstance->Position(glm::vec2(30.0f, 60.0f));
    _engine->Render();
    ExpectAllBackgroundColorOnScreen();
}

TEST_F(TileMapTests, Given8x4TileMapAtX30Y60_Camera8x6FovCenteredAtCenterOfTileMap_EntireTileMapIsVisible)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4, 0);

    // Move tile map to an off camera location and expect all background color
    tileMapInstance->Position(glm::vec2(30.0f, 60.0f));

    // Move the camera to re-center the tile map in the view and expect
    // to see the tile map as before
    _camera->Center(tileMapInstance->Position() + glm::vec3(tileMapInstance->Size() / 2.0f, 0.0f));
    _engine->Render();
    Expect8x4ColorTilesMapCenteredOnScreen();
}

TEST_F(TileMapTests, Given8x4TileMapAtOrigin_Camera8x6FovCenteredAtOrigin_Left4ColumnsAndBottom3RowsOfTileMapAreVisible)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4, 0);

    // Center the camera at the origin
    _camera->Center(glm::vec2(0.0f, 0.0f));
    _engine->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // First row is background color and 1st half of 2nd row of tile map
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            _tileMapImage8x4.TileColors[1][0], _tileMapImage8x4.TileColors[1][1], _tileMapImage8x4.TileColors[1][2], _tileMapImage8x4.TileColors[1][3] },

            // Second row is background color and 1st half of 3rd row of tile map
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            _tileMapImage8x4.TileColors[2][0], _tileMapImage8x4.TileColors[2][1], _tileMapImage8x4.TileColors[2][2], _tileMapImage8x4.TileColors[2][3] },

            // Third row is background color and 1st half of 4th row of tile map
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            _tileMapImage8x4.TileColors[3][0], _tileMapImage8x4.TileColors[3][1], _tileMapImage8x4.TileColors[3][2], _tileMapImage8x4.TileColors[3][3] },

            // Fourth row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Fifth row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Sixth row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(TileMapTests, Given8x4TileMapAtOriginRotated45Degrees_Camera8x6FovCenteredAtCenterOfTileMap_TODO)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4, 0);

    // Rotate the tile map 45 degrees
    tileMapInstance->Rotation(45.0f);
    _engine->Render();

    const unsigned int ColumnCount = 6;
    const unsigned int RowCount = 9;
    ExpectRotatedTileColorGridOnScreen<ColumnCount, RowCount>(
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            { ColorBackgroundUglyYellow, ColorOrange, ColorGreen, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            { ColorWhite, ColorWhite, ColorCyan, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            { ColorGreen, ColorBlue, ColorBlue, ColorRed, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            { ColorCyan, ColorLightGray, ColorLightGray, ColorLightGreen, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            { ColorBackgroundUglyYellow, ColorRed, ColorLightPeach, ColorLightPeach, ColorPurple, ColorBackgroundUglyYellow },

            { ColorBackgroundUglyYellow, ColorLightGreen, ColorBlack, ColorBlack, ColorLightPurple, ColorBackgroundUglyYellow },

            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorPurple, ColorMagenta, ColorMagenta, ColorSkyBlue },

            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorLightPurple, ColorDarkGray, ColorDarkGray, ColorBackgroundUglyYellow },

            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorSkyBlue, ColorOrange, ColorBackgroundUglyYellow },

        } }));

}

TEST_F(TileMapTests, Given8x4TileMapEmptyCentersAtOrigin_Camera8x6FovCenteredAtCenterOfTileMap_EntireTileMapIsVisibleAndCentersShowBackground)
{
    auto tileMapInstance = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMapEmptyCenters8x4, 0);
    _engine->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreenWithOrientationBrownAndSeparateCenterColors<ColumnCount, RowCount>(
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // First row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Middle 4 rows should be equal to the colors of the tiles in the tile atlas
            // as selected by TileMapImage8x4
            _tileMapImage8x4.TileColors[0],
            _tileMapImage8x4.TileColors[1],
            _tileMapImage8x4.TileColors[2],
            _tileMapImage8x4.TileColors[3],

            // Last row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } }),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // All centers are background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(TileMapTests, Given8x4TileMapEmptyCentersOnLayer1AndSolidTileMapFlippedTopToBottomOnLayer0_EntireLayer1TileMapIsVisibleAndCentersShowTileMapFromLayer0)
{
    auto tileMapInstanceLayer1 = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMapEmptyCenters8x4, 1);
    auto tileMapInstanceLayer0 = SetupTileMap8x4InstanceInCenterOfCamera8x6Fov(*_colorTilesMap8x4FlippedTopToBottom, 0);

    _engine->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreenWithOrientationBrownAndSeparateCenterColors<ColumnCount, RowCount>(
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // First row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Middle 4 rows should be equal to the colors of the tiles in the tile atlas
            // as selected by TileMapImage8x4
            _tileMapImage8x4.TileColors[0],
            _tileMapImage8x4.TileColors[1],
            _tileMapImage8x4.TileColors[2],
            _tileMapImage8x4.TileColors[3],

            // Last row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } }),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // First row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // Middle 4 rows should be equal to the colors of the tiles in the tile atlas
            // as selected by TileMapImage8x4FlippedTopToBottom
            _tileMapImage8x4FlippedTopToBottom.TileColors[0],
            _tileMapImage8x4FlippedTopToBottom.TileColors[1],
            _tileMapImage8x4FlippedTopToBottom.TileColors[2],
            _tileMapImage8x4FlippedTopToBottom.TileColors[3],

            // Last row is background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}
