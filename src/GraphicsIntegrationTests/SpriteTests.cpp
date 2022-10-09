#include <gtest/gtest.h>

#include <jkengine/Graphics/Graphics.h>

#include <jkengine/Window/GlfwWindow.h>
#include <jkengine/Window/GlfwWrapper.h>

#include "ColorTiles4x4.h"
#include "ScreenSampleHelpers.h"

using namespace testing;
using namespace Graphics;

class SpriteTests : public Test
{
public:
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 600;
    static constexpr float AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
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
    Window::GlfwWrapper _glfw;
    Window::GlfwWindow _window;
    std::unique_ptr<IEngine> _engine;

    struct SceneWithOneSprite
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterBuildPtr<ISprite> sprite;
    };

    SceneWithOneSprite SetupSceneWithOneSprite()
    {
        SceneWithOneSprite result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageColorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageColorTiles4x4EmptyCenters,
            glm::vec2(4.0f, 4.0f)
        };

        tileAtlas.AddSprite(SpriteDefinition{&result.sprite, 0});
        sceneDefinition.AddTileAtlas(tileAtlas);

        result.scene = _engine->CreateScene(sceneDefinition);
        result.scene->ClearColor(ColorBackgroundUglyYellow);
        result.camera = result.scene->Camera2d();
        return result;
    }

    struct SceneWithTwoSpritesOnDifferentLayers
    {
        std::unique_ptr<IScene> scene;
        ICamera2d* camera;
        AfterBuildPtr<ISprite> spriteOnLayer0;
        AfterBuildPtr<ISprite> spriteOnLayer1;
    };

    SceneWithTwoSpritesOnDifferentLayers SetupSceneWithTwoSpritesOnDifferentLayers()
    {
        SceneWithTwoSpritesOnDifferentLayers result{};
        SceneDefinition sceneDefinition{DrawingLayers};

        PngImage imageColorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        TileAtlasDefinition tileAtlas{
            DrawingLayers,
            &imageColorTiles4x4EmptyCenters,
            glm::vec2(4.0f, 4.0f)
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

TEST_F(SpriteTests, GivenSceneWithTwoSprites_NoSpritesVisibleByDefault)
{
    auto setup = SetupSceneWithTwoSpritesOnDifferentLayers();
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

TEST_F(SpriteTests, Given3x3SpriteAtOriginWithAtlasLocationX0Y0_Camera8x6FovCenteredAtOrigin_EntireSpriteIsVisible)
{
    auto setup = SetupSceneWithOneSprite();
    setup.sprite->Show(true);
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

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
    setup.sprite->Show(true);
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

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
    setup.sprite->Show(true);
    setup.sprite->Position(glm::vec2(30.0f, 60.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(6.0f, 6.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(33.0f, 63.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

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
    setup.sprite->Show(true);
    setup.sprite->Position(glm::vec2(30.0f, 60.0f));
    setup.sprite->Rotation(0.0f);
    setup.sprite->Size(glm::vec2(6.0f, 6.0f));
    setup.sprite->AtlasLocation(GridLocation(2, 1));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

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
    setup.sprite->Show(true);
    setup.sprite->Position(glm::vec2(0.0f, 0.0f));
    setup.sprite->Rotation(45.0f);
    setup.sprite->Size(glm::vec2(3.0f, 3.0f));
    setup.sprite->AtlasLocation(GridLocation(0, 0));

    setup.camera->Center(glm::vec2(1.5f, 1.5f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

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
    auto setup = SetupSceneWithTwoSpritesOnDifferentLayers();
    setup.spriteOnLayer1->Show(true);
    setup.spriteOnLayer1->Position(glm::vec2(0.0f, 0.0f));
    setup.spriteOnLayer1->Rotation(0.0f);
    setup.spriteOnLayer1->Size(glm::vec2(3.0f, 3.0f));
    setup.spriteOnLayer1->AtlasLocation(GridLocation(0, 0));

    setup.spriteOnLayer0->Show(true);
    setup.spriteOnLayer0->Position(glm::vec2(-1.0f, 0.0f));
    setup.spriteOnLayer0->Rotation(0.0f);
    setup.spriteOnLayer0->Size(glm::vec2(3.0f, 3.0f));
    setup.spriteOnLayer0->AtlasLocation(GridLocation(1, 0));

    setup.camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    setup.camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

    // Note if you look at this manually, there is a narrow bit of the
    // center "sub-tiles" from the layer0 sprite visible left of center.
    // this is because the sizes of the tiles in the tile atlas are not
    // evenly divisible into 3 "sub-tiles".  The first two rows and
    // columns are 21 pixels and the last row and column is 22 pixels.
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
