#include <gtest/gtest.h>

#include <JkEng/Graphics/Graphics.h>

#include <JkEng/Window/GlfwWindow.h>
#include <JkEng/Window/GlfwWrapper.h>

#include "ColorTiles4x4.h"
#include "ScreenSampleHelpers.h"

using namespace testing;
using namespace JkEng;
using namespace JkEng::Graphics;

class SpriteTests : public Test
{
public:
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 600;
    static constexpr size_t DrawingLayers = 3;

    SpriteTests()
      : _libPng(),
        _glfw(),
        _window(_glfw, WindowWidth, WindowHeight, "SpriteTests"),
        _engine(std::make_unique<OpenGL::Engine>(_window))
    {
    }

protected:
    LibPngWrapper _libPng;
    JkEng::Window::GlfwWrapper _glfw;
    JkEng::Window::GlfwWindow _window;
    std::unique_ptr<IEngine> _engine;

    struct SceneWithOneSprite
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterCreatePtr<ISprite> sprite;
    };

    SceneWithOneSprite SetupSceneWithOneSprite()
    {
        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageColorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageColorTiles4x4EmptyCenters,
            glm::vec2(4.0f, 4.0f),
            glm::vec2(0.0f, 0.0f)
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }

    SceneWithOneSprite SetupSceneWithBlackRedWhiteBlueSquare()
    {
        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage image(&_libPng, "TestFiles/SingleTileBlackRedWhiteBlue.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &image,
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f / 16.0f, 1.0f / 16.0f)
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }

    SceneWithOneSprite SetupSceneWithOneAllBlackSpriteFrom16x16CheckerBoardAtlasWith1PixelBorder()
    {
        // Border size is 1 pixel on each edge of each of the 16x16 pixel tiles
        constexpr glm::vec2 eachTileBorderThicknessInTiles(1.0f / 16.0f, 1.0f / 16.0f);

        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageBlackAndWhiteCheckerBoard16x16(
            &_libPng,
            "TestFiles/BlackAndWhiteCheckerBoard2TilesBy2Tiles16x16.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageBlackAndWhiteCheckerBoard16x16,
            glm::vec2(2.0f, 2.0f),
            eachTileBorderThicknessInTiles
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.camera = result.scene->Camera2d();
        return result;
    }

    SceneWithOneSprite SetupSceneWithOneAllBlackSpriteFrom64x64CheckerBoardAtlasWith1PixelBorder()
    {
        // Border size is 1 pixel on each edge of each of the 64x64 pixel tiles
        constexpr glm::vec2 eachTileBorderThicknessInTiles(1.0f / 64.0f, 1.0f / 64.0f);

        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageBlackAndWhiteCheckerBoard64x64(
            &_libPng,
            "TestFiles/BlackAndWhiteCheckerBoard2TilesBy2Tiles64x64.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageBlackAndWhiteCheckerBoard64x64,
            glm::vec2(2.0f, 2.0f),
            eachTileBorderThicknessInTiles
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.camera = result.scene->Camera2d();
        return result;
    }

    SceneWithOneSprite SetupSceneWithOneTestPatternSpriteFrom16x16AtlasWith1PixelBorder()
    {
        // Border size is 1 pixel on each edge of each of the 16x16 pixel tiles
        constexpr glm::vec2 eachTileBorderThicknessInTiles(1.0f / 16.0f, 1.0f / 16.0f);

        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageTestPatternWithBorder(
            &_libPng,
            "TestFiles/16x16WithBorderTestPatterns.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageTestPatternWithBorder,
            glm::vec2(2.0f, 2.0f),
            eachTileBorderThicknessInTiles
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.camera = result.scene->Camera2d();
        return result;
    }

    struct SceneWithTwoSpritesOnDifferentLayers
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterCreatePtr<ISprite> spriteOnLayer0;
        AfterCreatePtr<ISprite> spriteOnLayer1;
    };

    SceneWithTwoSpritesOnDifferentLayers SetupSceneWithTwoSpritesOnDifferentLayers()
    {
        SceneWithTwoSpritesOnDifferentLayers result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageColorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageColorTiles4x4EmptyCenters,
            glm::vec2(4.0f, 4.0f),
            glm::vec2(0.0f, 0.0f)
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.spriteOnLayer0, 0});
        tileAtlas.AddSprite(SpriteDefinition{&result.spriteOnLayer1, 1});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }
};

TEST_F(SpriteTests, Given3x3SpriteAtOriginWithAtlasLocationX0Y0_Camera8x6FovCenteredAtOriginAndShowCalledWithFalse_NothingVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Show(false);
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    constexpr unsigned int ColumnCount = 32;
    constexpr unsigned int RowCount = 24;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        OneColorGrid<ColumnCount, RowCount>(ColorBackgroundUglyYellow)
    );
}

TEST_F(SpriteTests, Given3x3SpriteAtOriginWithAtlasLocationX0Y0_Camera8x6FovCenteredAtOrigin_EntireSpriteIsVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color except for 3 cells right of center
            // The first of these three should be brown which is the orientation mark
            // on the tiles and the other two should be white
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorOrientationBrown, ColorWhite, ColorWhite, ColorBackgroundUglyYellow },

            // 2nd row is all background color except for 2 of the 3 cells right of center
            // but the middle of the 3 cells right of center is actually be background color
            // because it is transparent in the sprite
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorWhite, ColorBackgroundUglyYellow, ColorWhite, ColorBackgroundUglyYellow },

            // 2rd row is all background color except for 3 cells right of center
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorWhite, ColorWhite, ColorWhite, ColorBackgroundUglyYellow },

            // 4th through 6th rows are all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(SpriteTests, Given3x3SpriteAtOriginWithAtlasLocationX2Y1_Camera8x6FovCenteredAtOrigin_EntireSpriteIsVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color except for 3 cells right of center
            // The first of these three should be brown which is the orientation mark
            // on the tiles and the other two should be cyan
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorOrientationBrown, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },

            // 2nd row is all background color except for 2 of the 3 cells right of center
            // but the middle of the 3 cells right of center is actually be background color
            // because it is transparent in the sprite
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorCyan, ColorBackgroundUglyYellow, ColorCyan, ColorBackgroundUglyYellow },

            // 2rd row is all background color except for 3 cells right of center
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorCyan, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },

            // 4th through 6th rows are all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(SpriteTests, Given6x6SpriteAtX30Y60_Camera8x6FovCenteredAtCenterOfSprite_EntireSpriteIsVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Position(glm::vec2(30.0f, 60.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(6.0f, 6.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(33.0f, 63.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    auto scr = _engine->TakeScreenshot();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *scr,
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st and 2nd rows have background color on both sides with the sprite visible
            // in the middle.  The part of the sprite in these rows is mostly cyan, except
            // for the orientation block in the upper left
            { ColorBackgroundUglyYellow, ColorOrientationBrown, ColorOrientationBrown, ColorCyan,
            ColorCyan, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorOrientationBrown, ColorOrientationBrown, ColorCyan,
            ColorCyan, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },

            // 3rd and 4th rows have background color on both sides with the sprite visible
            // in the middle.  The part of the sprite in these rows is mostly cyan, except
            // for the center of the sprite which is in the center of both rows and is 
            // transparent so shows background color.
            //
            // Note that the transparent block is slightly off center in image, the colored
            // part to the left of it is 21 pixels, the transparent part is 21 pixels, but
            // the colored part to the right is 22 pixels.  This caused me to need to change
            // the sampleDistanceFromEdge in the verification function to be width/8 rather
            // than width / 16
            { ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },

            // 5th and 6th rows have background color on both sides with the sprite visible
            // in the middle.  The part of the sprite in these rows is all cyan.
            { ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorCyan,
            ColorCyan, ColorCyan, ColorCyan, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorCyan, ColorCyan, ColorCyan,
            ColorCyan, ColorCyan, ColorCyan, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(SpriteTests, Given6x6Sprite_GivenOffCameraAtX4Y0_SpriteIsNotVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Position(glm::vec2(30.0f, 60.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(6.0f, 6.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    auto scr = _engine->TakeScreenshot();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *scr,
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // Expect all background color
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
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
        } })
    );
}

TEST_F(SpriteTests, Given3x3SpriteAtOriginRotated45Degrees_Camera8x6FovCenteredOnSprite_EntireSpriteIsVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(45.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));

    setup.camera->Center(glm::vec2(1.5f, 1.5f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    auto screenshot = _engine->TakeScreenshot();

    auto width = static_cast<float>(screenshot->Width());
    auto height = static_cast<float>(screenshot->Height());

    // Calculate diagonal of "sub-squares" of the sprite based on the following
    // information:
    // - The sprite is 3 in world coordinates wide
    // - The window is 8 in world coordinates wide
    // - The sprite is square
    // - The sprite is essentially split into 9 "sub-squares" and we want
    //   to test a pixel at the center of each of these "sub-squares".
    auto pixelsPerWorldUnit = width / 8.0f;
    auto spriteEdge = 3.0f * pixelsPerWorldUnit;
    auto spriteDiagonal = std::sqrt(2.0f) * spriteEdge;
    auto subsquareDiagonal = spriteDiagonal / 3.0f;

    auto centerSubsquareX = width / 2.0f;
    auto centerSubsquareY = height / 2.0f;

    // sample near the corners of the sprite to spot check its location

    ExpectColorAtScreenPosition(*screenshot, "above sprite top",
        ColorBackgroundUglyYellow, centerSubsquareX, centerSubsquareY - 2.0f * subsquareDiagonal, 0, 0);

    ExpectColorAtScreenPosition(*screenshot, "left of sprite top",
        ColorBackgroundUglyYellow, centerSubsquareX - 1.0f * subsquareDiagonal, centerSubsquareY - 1.0f * subsquareDiagonal, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "top subsquare",
        ColorOrientationBrown, centerSubsquareX, centerSubsquareY - 1.0f * subsquareDiagonal, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "right of sprite top",
        ColorBackgroundUglyYellow, centerSubsquareX + 1.0f * subsquareDiagonal, centerSubsquareY - 1.0f * subsquareDiagonal, 0, 0);

    ExpectColorAtScreenPosition(*screenshot, "left of sprite corner",
        ColorBackgroundUglyYellow, centerSubsquareX - 2.0f * subsquareDiagonal, centerSubsquareY, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "left of center subsquare",
        ColorWhite, centerSubsquareX - 1.0f * subsquareDiagonal, centerSubsquareY, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "center subsquare",
        ColorBackgroundUglyYellow, centerSubsquareX, centerSubsquareY, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "right of center subsquare",
        ColorWhite, centerSubsquareX + 1.0f * subsquareDiagonal, centerSubsquareY, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "right of sprite corner",
        ColorBackgroundUglyYellow, centerSubsquareX + 2.0f * subsquareDiagonal, centerSubsquareY, 0, 0);

    ExpectColorAtScreenPosition(*screenshot, "left of sprite bottom",
        ColorBackgroundUglyYellow, centerSubsquareX - 1.0f * subsquareDiagonal, centerSubsquareY + 1.0f * subsquareDiagonal, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "bottom subsquare",
        ColorWhite, centerSubsquareX, centerSubsquareY + 1.0f * subsquareDiagonal, 0, 0);
    ExpectColorAtScreenPosition(*screenshot, "right of sprite bottom",
        ColorBackgroundUglyYellow, centerSubsquareX + 1.0f * subsquareDiagonal, centerSubsquareY + 1.0f * subsquareDiagonal, 0, 0);

    ExpectColorAtScreenPosition(*screenshot, "below sprite bottom",
        ColorBackgroundUglyYellow, centerSubsquareX, centerSubsquareY + 2.0f * subsquareDiagonal, 0, 0);

}

TEST_F(SpriteTests, GivenTwo3x3SpritesOnDifferentLayers_TransparentAreasOfHigherLayerSpriteShowLowerLayerSprite)
{
    // Note: when watching this test run it looks a bit odd visually because there
    // is a vertical red line showing between blocks of two other colors.
    // This is okay and just because there is a narrow bit of the
    // center "sub-tiles" from the layer0 sprite visible left of center.
    // this is because the sizes of the tiles in the tile atlas are not
    // evenly divisible into 3 "sub-tiles".  The first two rows and
    // columns are 21 pixels and the last row and column is 22 pixels.

    auto setup = SetupSceneWithTwoSpritesOnDifferentLayers();
    setup.spriteOnLayer1->Position(glm::vec2(0.0f, 0.0f));
    setup.spriteOnLayer1->Rotation(0.0f);
    setup.spriteOnLayer1->Size(glm::vec2(3.0f, 3.0f));
    setup.spriteOnLayer1->AtlasLocation(GridLocation(0, 0));

    setup.spriteOnLayer0->Position(glm::vec2(-1.0f, 0.0f));
    setup.spriteOnLayer0->Rotation(0.0f);
    setup.spriteOnLayer0->Size(glm::vec2(3.0f, 3.0f));
    setup.spriteOnLayer0->AtlasLocation(GridLocation(1, 0));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.scene->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color except for 3 cells right of center from
            // layer 1 sprite and one cell left of center from layer 0 sprite
            // layer 0 sprite: visible cell in this row is the brown orientation mark
            // layer 1 sprite: The first of these three should be brown which is the
            // orientation mark on the tiles and the other two should be white
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorOrientationBrown,
            ColorOrientationBrown, ColorWhite, ColorWhite, ColorBackgroundUglyYellow },

            // 2nd row is all background color except for 2 of the 3 cells right of center
            // from the layer 1 sprite and one cell left of center as well as one right of
            // center (visible through transparent part of layer 1 sprite) from the layer 0 sprite
            // layer 0 sprite: visible cells are red
            // layer 1 sprite: the middle of the 3 cells right of center is actually be
            // red because it is transparent and the layer 1 sprite is behind it the others will be white
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorRed,
            ColorWhite, ColorRed, ColorWhite, ColorBackgroundUglyYellow },

            // 2rd row is all background color except for 3 cells right of center for the layer 1
            // sprite and one cell left of center from the layer 0 sprite
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorRed,
            ColorWhite, ColorWhite, ColorWhite, ColorBackgroundUglyYellow },

            // 4th through 6th rows are all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }
        } })
    );
}

TEST_F(SpriteTests, GivenAllBlackSpriteFrom16x16CheckerBoardTileAtlasWith1PixelBorderThickness_CameraAtProblematicOffset_ScreenIsAllBlack)
{
    // Note to validate that this test would fail on some particular hardware
    // if the overscanning problem is present, zero out the border in the
    // TileAtlasDefinition in the setup method.
    auto setup = SetupSceneWithOneAllBlackSpriteFrom16x16CheckerBoardAtlasWith1PixelBorder();
  
    // All Black Background so we can assert that the entire window to be black
    setup.scene->ClearColor(ColorBlack);

    setup.sprite->Position(glm::vec2(1.0f, 1.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(1.0f, 1.0f));
    setup.sprite->AtlasLocation(Graphics::GridLocation(0, 0));

    setup.camera->FieldOfView(Graphics::ICamera2d::Fov(
            -4.0f, 4.0f,
            -3.0f, 3.0f));

    // What does the "Problematic Offset" mean?
    //
    // These values were determined to cause the issue on
    // my mid-2014 13" MacBook Pro running Big Sur 11.6.8
    // with an Intel Iris 1536 MB.  On this machine the
    // 800x600 window is really 1600 by 1200 on screen.
    // See docs/notes/TileMapOverscanBugNotes.md for more notes.
    //
    // The problem seems to be caused by having many screen pixels
    // per texture pixel and scrolling to an offset the right amount
    // just below or just above an integer number of texture pixels
    // from the origin.
    //
    // These values are approximately in the middle of the range of
    // values that had the problem.
    setup.camera->Center(glm::vec2(
        0x1.p-4 - 68 * 0x1.p-20,
        0x1.p-4 + 73 * 0x1.p-20));

    setup.scene->Render();

    auto screenshot = _engine->TakeScreenshot();

    for (unsigned int y = 0; y < screenshot->Height(); y++)
    {
        for (unsigned int x = 0; x < screenshot->Width(); x++)
        {
            screenshot->GetPixel(x, y);

            ExpectColorAtScreenPosition(
                *screenshot, "", Color(0, 0, 0, 255), x, y, 0, 0);
        }
    }
}

TEST_F(SpriteTests, GivenAllBlackSpriteFrom64x64CheckerBoardTileAtlasWith1PixelBorderThickness_CameraAtProblematicOffset_ScreenIsAllBlack)
{
    // Note to validate that this test would fail on some particular hardware
    // if the overscanning problem is present, zero out the border in the
    // TileAtlasDefinition in the setup method.
    auto setup = SetupSceneWithOneAllBlackSpriteFrom64x64CheckerBoardAtlasWith1PixelBorder();
  
    // All Black Background so we can assert that the entire window to be black
    setup.scene->ClearColor(ColorBlack);

    setup.sprite->Position(glm::vec2(1.0f, 1.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(1.0f, 1.0f));
    setup.sprite->AtlasLocation(Graphics::GridLocation(0, 0));

    setup.camera->FieldOfView(Graphics::ICamera2d::Fov(
            -1.0f, 1.0f,
            -0.75f, 0.75f));

    // What does the "Problematic Offset" mean?
    //
    // These values were determined to cause the issue on
    // my mid-2014 13" MacBook Pro running Big Sur 11.6.8
    // with an Intel Iris 1536 MB.  On this machine the
    // 800x600 window is really 1600 by 1200 on screen.
    // See docs/notes/TileMapOverscanBugNotes.md for more notes.
    //
    // The problem seems to be caused by having many screen pixels
    // per texture pixel and scrolling to an offset the right amount
    // just below or just above an integer number of texture pixels
    // from the origin.
    //
    // These values are approximately in the middle of the range of
    // values that had the problem.
    setup.camera->Center(glm::vec2(
        1.5f + 0x1.p-6 - 68 * 0x1.p-22,
        1.5f + 0x1.p-6 + 73 * 0x1.p-22));

    setup.scene->Render();

    auto screenshot = _engine->TakeScreenshot();

    for (unsigned int y = 0; y < screenshot->Height(); y++)
    {
        for (unsigned int x = 0; x < screenshot->Width(); x++)
        {
            screenshot->GetPixel(x, y);

            ExpectColorAtScreenPosition(
                *screenshot, "", Color(0, 0, 0, 255), x, y, 0, 0);
        }
    }
}

TEST_F(SpriteTests, GivenTestPatternSpriteWithOnePixelBorder_CorrectPortionIsVisible)
{
    auto setup = SetupSceneWithOneTestPatternSpriteFrom16x16AtlasWith1PixelBorder();
  
    // All white background so black sprite border is clearly defined
    setup.scene->ClearColor(ColorWhite);

    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(1.0f, 1.0f));
    setup.sprite->AtlasLocation(Graphics::GridLocation(1, 1));

    setup.camera->FieldOfView(Graphics::ICamera2d::Fov(
            -12.0f / 14.0f, 12.0f / 14.0f,
            -9.0f / 14.0f, 9.0f / 14.0f));

    setup.camera->Center(glm::vec2( -1.0f / 14.0f + 12.0f / 14.0f, -3.0f / 14.0f + 9.0f / 14.0f ));
    setup.scene->Render();

    auto screenshot = _engine->TakeScreenshot();
    screenshot->SaveToFileAsRaw("testPattern.data");

    const unsigned int ColumnCount = 24;
    const unsigned int RowCount = 18;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            { ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack,
              ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack,
              ColorBlack, ColorBlack, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorRed, ColorRed, ColorRed, ColorRed,
              ColorRed, ColorRed, ColorRed, ColorRed, ColorRed, ColorRed,
              ColorRed, ColorRed, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlue, ColorBlue,
              ColorBlue, ColorBlue, ColorBlue, ColorBlue, ColorBlue, ColorBlue,
              ColorBlue, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorBlue, ColorBlack, ColorBlue,
              ColorBlack, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlue, ColorBlue, ColorBlue, ColorBlue,
              ColorBlue, ColorBlue, ColorBlue, ColorBlue, ColorBlue, ColorBlue,
              ColorBlue, ColorBlue, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack,
              ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack, ColorBlack,
              ColorBlack, ColorBlack, ColorBlack, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
            { ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite,
              ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite, ColorWhite },
        } })
    );
}

TEST_F(SpriteTests, Sprite4x4CenteredInFov_GivenMirrorCalledWithXTrueYFalse_SpriteIsMirroredHorizontally)
{
    auto setup = SetupSceneWithBlackRedWhiteBlueSquare();
    setup.sprite->Position(glm::vec2(-2.0f, -2.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(4.0f, 4.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));
    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.sprite->Mirror(true, false);

    setup.scene->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
              ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 2nd row has red and black centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorRed, ColorRed,
              ColorBlack, ColorBlack, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 3rd row also has red and black centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorRed, ColorRed,
              ColorBlack, ColorBlack, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 4th row has white and blue centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorWhite, ColorWhite,
              ColorBlue, ColorBlue, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 5th row also has white and blue centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorWhite, ColorWhite,
              ColorBlue, ColorBlue, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 6th row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
              ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }

        } })
    );
}

TEST_F(SpriteTests, Sprite4x4CenteredInFov_GivenMirrorCalledWithXFalseYTrue_SpriteIsMirroredHorizontally)
{
    auto setup = SetupSceneWithBlackRedWhiteBlueSquare();
    setup.sprite->Position(glm::vec2(-2.0f, -2.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(4.0f, 4.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));
    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -3.0f, 3.0f));

    setup.sprite->Mirror(false, true);

    setup.scene->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
              ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 2nd row has blue and white centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBlue, ColorBlue,
              ColorWhite, ColorWhite, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 3rd row also has blue and white centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBlue, ColorBlue,
              ColorWhite, ColorWhite, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 4th row has black and red centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBlack, ColorBlack,
              ColorRed, ColorRed, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 5th row also has black and red centered
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBlack, ColorBlack,
              ColorRed, ColorRed, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow },

            // 6th row is all background color
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
              ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow }

        } })
    );
}
