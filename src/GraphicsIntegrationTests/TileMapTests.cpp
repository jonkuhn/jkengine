#include <array>
#include <cmath>
#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include <gtest/gtest.h>

#include <jkengine/Graphics/Graphics.h>
#include <jkengine/Window/GlfwWindow.h>
#include <jkengine/Window/GlfwWrapper.h>

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
        _glfw(),
        _window(_glfw, WindowWidth, WindowHeight, "TileMapTests"),
        _engine(std::make_unique<OpenGL::Engine>(_window)),
        _scene(),
        _camera(nullptr),
        _tileAtlasImageColorTiles4x4(&_libPng, "TestFiles/colortiles4x4.png"),
        _tileAtlasImageColorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png"),
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
        )
    {
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage colorTiles4x4(&_libPng, "TestFiles/colortiles4x4.png");
        TileAtlasDefinition atlasColorTiles4x4{
            DrawingLayers,
            &colorTiles4x4,
            glm::vec2(4.0f, 4.0f)
        };
        atlasColorTiles4x4.AddTileMap(TileMapDefinition{
            &_tileMapSolid8x4Layer0,
            0,
            &_tileMapImage8x4
        });
        atlasColorTiles4x4.AddTileMap(TileMapDefinition{
            &_tileMapSolid8x4FlippedTopToBottomLayer0,
            0,
            &_tileMapImage8x4FlippedTopToBottom
        });

        sceneDefinition.AddTileAtlas(atlasColorTiles4x4);

        PngImage colorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        TileAtlasDefinition atlasColorTilesEmptyCenters4x4{
            DrawingLayers,
            &colorTiles4x4EmptyCenters,
            glm::vec2(4.0f, 4.0f)
        };
        atlasColorTilesEmptyCenters4x4.AddTileMap(TileMapDefinition{
            &_tileMapEmptyCenters8x4Layer0,
            0,
            &_tileMapImage8x4
        });

        atlasColorTilesEmptyCenters4x4.AddTileMap(TileMapDefinition{
            &_tileMapEmptyCenters8x4Layer1,
            1,
            &_tileMapImage8x4
        });

        sceneDefinition.AddTileAtlas(atlasColorTilesEmptyCenters4x4);

        _scene = _engine->CreateScene(sceneDefinition);
        _scene->ClearColor(ColorBackgroundUglyYellow);
        _camera = _scene->Camera2d();
    }
protected:
    LibPngWrapper _libPng;
    Window::GlfwWrapper _glfw;
    Window::GlfwWindow _window;
    std::unique_ptr<IEngine> _engine;
    std::unique_ptr<IScene> _scene;
    ICamera2d* _camera;
    PngImage _tileAtlasImageColorTiles4x4;
    PngImage _tileAtlasImageColorTiles4x4EmptyCenters;
    TileMapImage8x4 _tileMapImage8x4;
    TileMapImage8x4 _tileMapImage8x4FlippedTopToBottom;
    AfterBuildPtr<ITileMap> _tileMapSolid8x4Layer0;
    AfterBuildPtr<ITileMap> _tileMapEmptyCenters8x4Layer0;
    AfterBuildPtr<ITileMap> _tileMapEmptyCenters8x4Layer1;
    AfterBuildPtr<ITileMap> _tileMapSolid8x4FlippedTopToBottomLayer0;

    struct SceneWithOneTileMap
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterBuildPtr<ITileMap> tileMap;
    };

    SceneWithOneTileMap SetupSceneWithOneTileMap(
        const IImage& tileAtlasImage,
        const IImage& tileMapImage)
    {
        SceneWithOneTileMap result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &tileAtlasImage,
            glm::vec2(4.0f, 4.0f)
        };

        tileAtlas.AddTileMap(TileMapDefinition{
            &result.tileMap,
            0,
            &tileMapImage
        });
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }

    struct SceneWithTwoTileMapsOnDifferentLayers
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterBuildPtr<ITileMap> tileMapOnLayer0;
        AfterBuildPtr<ITileMap> tileMapOnLayer1;
    };

    SceneWithTwoTileMapsOnDifferentLayers SetupSceneWithTwoTileMapsOnDifferentLayers(
        const IImage& tileAtlasImage0,
        const IImage& tileMapImage0,
        const IImage& tileAtlasImage1,
        const IImage& tileMapImage1)
    {
        SceneWithTwoTileMapsOnDifferentLayers result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        TileAtlasDefinition tileAtlas0{
            DrawingLayers,
            &tileAtlasImage0,
            glm::vec2(4.0f, 4.0f)
        };

        tileAtlas0.AddTileMap(TileMapDefinition{
            &result.tileMapOnLayer0,
            0,
            &tileMapImage0
        });
        sceneDefinition.AddTileAtlas(tileAtlas0);

        TileAtlasDefinition tileAtlas1{
            DrawingLayers,
            &tileAtlasImage1,
            glm::vec2(4.0f, 4.0f)
        };

        tileAtlas1.AddTileMap(TileMapDefinition{
            &result.tileMapOnLayer1,
            1,
            &tileMapImage1
        });
        sceneDefinition.AddTileAtlas(tileAtlas1);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }
    
    void SetupTileMap8x4InCenterOfCamera8x6Fov(ITileMap* tileMap, ICamera2d* camera)
    {
        // Create instance of 8x4 tile map with lower left at origin
        // with no rotation and size in world coordinates set to
        // the width and height of the tile map (8x4)
        tileMap->Position(glm::vec2(0.0f, 0.0f));
        tileMap->Rotation(0.0f);
        tileMap->Size(glm::vec2(
            static_cast<float>(_tileMapImage8x4.Width()),
            static_cast<float>(_tileMapImage8x4.Height())));

        // Center camera at the center of the tile map
        // (since lower left of tile map is at origin, taking half
        // of the width and height of the tile map will give the
        // center point)
        camera->Center(tileMap->Size() / 2.0f);

        // Set FoV so that 8 tiles are visible horizontally and due
        // to the aspect ration 6 tiles will be visible vertically
        camera->FieldOfView(ICamera2d::Fov(
            -4.0f, 4.0f,
            -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));
    }

    template<unsigned int ColumnCount, unsigned int RowCount>
    void ExpectTileColorGridOnScreenWithOrientationBrownAndSeparateCenterColors(
        std::array<std::array<Color, ColumnCount>, RowCount> expectedTileColors,
        std::array<std::array<Color, ColumnCount>, RowCount> expectedCenterColors)
    {
        auto screenshot = _engine->TakeScreenshot();
        auto columnWidth = static_cast<float>(screenshot->Width()) / static_cast<float>(ColumnCount);
        auto rowHeight = static_cast<float>(screenshot->Height()) / static_cast<float>(RowCount);
        auto sampleDistanceFromEdgeX = columnWidth / 8.0f;
        auto sampleDistanceFromEdgeY = rowHeight / 8.0f;

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

                ExpectColorAtScreenPosition(*screenshot, "upper left",
                    expectedUpperLeftColor, x + sampleLeftX, y + sampleUpperY, row, column);

                ExpectColorAtScreenPosition(*screenshot, "upper right",
                    expectedColor, x + sampleRightX, y + sampleUpperY, row, column);

                auto expectedCenterColor = expectedCenterColors[row][column];
                ExpectColorAtScreenPosition(*screenshot, "center",
                    expectedCenterColor, x + sampleCenterX, y + sampleCenterY, row, column);

                ExpectColorAtScreenPosition(*screenshot, "lower left",
                    expectedColor, x + sampleLeftX, y + sampleLowerY, row, column);

                ExpectColorAtScreenPosition(*screenshot, "lower right",
                    expectedColor, x + sampleRightX, y + sampleLowerY, row, column);

            }
        }
    }

    template<unsigned int ColumnCount, unsigned int RowCount>
    void ExpectRotatedTileColorGridOnScreen(
        std::array<std::array<Color, ColumnCount>, RowCount> expectedTileColors)
    {
        auto screenshot = _engine->TakeScreenshot();

        // Assume non-rotated tiles fit exactly 8 across for calculating
        // tileDiagonal
        auto tileDiagonal = sqrt(2.0f) * static_cast<float>(screenshot->Width()) / 8.0f;

        auto columnWidth = tileDiagonal;
        auto rowHeight = tileDiagonal / 2;

        auto firstCenterX = (screenshot->Width() / 2) - (5 * tileDiagonal / 2);
        auto firstCenterY = (screenshot->Height() / 2) - (2 * tileDiagonal);
        auto oddRowXOffset = tileDiagonal / 2;

        // sample each tile near 4 corners plus center
        auto sampleCenterX = 0.0f;
        auto sampleCenterY = 0.0f;

        auto sampleDistanceFromCorner = tileDiagonal / 16.0f;
        auto sampleLeftX = -0.5 * tileDiagonal + sampleDistanceFromCorner;
        auto sampleRightX = 0.5 * tileDiagonal - sampleDistanceFromCorner;
        auto sampleTopY = -0.5 * tileDiagonal + sampleDistanceFromCorner;
        auto sampleBottomY = 0.5 * tileDiagonal - sampleDistanceFromCorner;
        
        for(unsigned int row = 0; row < RowCount; row++)
        {
            for(unsigned int column = 0; column < ColumnCount; column++)
            {
                auto expectedColor = expectedTileColors[row][column];
                auto x = firstCenterX + column * columnWidth + ((row % 2) * oddRowXOffset);
                auto y = firstCenterY + row * rowHeight;

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "center", expectedColor,
                    x + sampleCenterX, y + sampleCenterY, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "left", expectedColor,
                    x + sampleLeftX, y + sampleCenterY, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "right", expectedColor,
                    x + sampleRightX, y + sampleCenterY, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "top", expectedColor,
                    x + sampleCenterX, y + sampleTopY, row, column);

                ExpectColorAtScreenPositionIfOnScreen(*screenshot, "bottom", expectedColor,
                    x + sampleCenterX, y + sampleBottomY, row, column);
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

TEST_F(TileMapTests, GivenSceneWithMultipleTileMaps_NoneVisibleByDefault)
{
    auto setup = SetupSceneWithTwoTileMapsOnDifferentLayers(
        _tileAtlasImageColorTiles4x4,
        _tileMapImage8x4FlippedTopToBottom,
        _tileAtlasImageColorTiles4x4EmptyCenters,
        _tileMapImage8x4
    );
    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

    setup.scene->Render();

    constexpr unsigned int ColumnCount = 32;
    constexpr unsigned int RowCount = 24;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        OneColorGrid<ColumnCount, RowCount>(ColorBackgroundUglyYellow)
    );
}

TEST_F(TileMapTests, Given8x4TileMapAtOrigin_Camera8x6FovCenteredAtCenterOfTileMap_EntireTileMapIsVisible)
{
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);
    setup.scene->Render();
    Expect8x4ColorTilesMapCenteredOnScreen();
}


TEST_F(TileMapTests, Given8x4TileMapOffCamera_Camera8x6FovCenteredAtCenterOfTileMap_EntireAllBackgroundColorOnScreen)
{
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);

    // Move tile map to an off camera location and expect all background color
    setup.tileMap->Position(glm::vec2(30.0f, 60.0f));
    setup.scene->Render();
    ExpectAllBackgroundColorOnScreen();
}

TEST_F(TileMapTests, Given8x4TileMapAtX30Y60_Camera8x6FovCenteredAtCenterOfTileMap_EntireTileMapIsVisible)
{
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);

    // Move tile map to an off camera location and expect all background color
    setup.tileMap->Position(glm::vec2(30.0f, 60.0f));

    // Move the camera to re-center the tile map in the view and expect
    // to see the tile map as before
    setup.camera->Center(setup.tileMap->Position() + glm::vec3(setup.tileMap->Size() / 2.0f, 0.0f));
    setup.scene->Render();
    Expect8x4ColorTilesMapCenteredOnScreen();
}

TEST_F(TileMapTests, Given8x4TileMapAtOrigin_Camera8x6FovCenteredAtOrigin_Left4ColumnsAndBottom3RowsOfTileMapAreVisible)
{
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);

    // Center the camera at the origin
    setup.camera->Center(glm::vec2(0.0f, 0.0f));
    setup.scene->Render();

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
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);

    // Rotate the tile map 45 degrees
    setup.tileMap->Rotation(45.0f);
    setup.scene->Render();

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
    auto setup = SetupSceneWithOneTileMap(_tileAtlasImageColorTiles4x4EmptyCenters, _tileMapImage8x4);
    setup.tileMap->Show(true);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMap.Get(), setup.camera);
    setup.scene->Render();

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
    auto setup = SetupSceneWithTwoTileMapsOnDifferentLayers(
        _tileAtlasImageColorTiles4x4,
        _tileMapImage8x4FlippedTopToBottom,
        _tileAtlasImageColorTiles4x4EmptyCenters,
        _tileMapImage8x4
    );

    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMapOnLayer1.Get(), setup.camera);
    SetupTileMap8x4InCenterOfCamera8x6Fov(setup.tileMapOnLayer0.Get(), setup.camera);
    setup.tileMapOnLayer1->Show(true);
    setup.tileMapOnLayer0->Show(true);

    setup.scene->Render();

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
