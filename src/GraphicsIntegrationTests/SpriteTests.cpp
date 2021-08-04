#include <gtest/gtest.h>

#include "Graphics/Graphics.h"

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
    static constexpr unsigned int DrawingLayers = 3;

    SpriteTests()
      : _libPng(),
        _engine(std::make_unique<OpenGL::Engine>(WindowWidth, WindowHeight, "SpriteTests", DrawingLayers)),
        _camera(_engine->Camera2d()),
        _atlasColorTilesEmptyCenters4x4()
    {
        _engine->ClearColor(ColorBackgroundUglyYellow);
        PngImage colorTiles4x4EmptyCenters(&_libPng, "TestFiles/colortiles4x4emptycenters.png");
        _atlasColorTilesEmptyCenters4x4 = _engine->CreateTileAtlas(colorTiles4x4EmptyCenters, glm::vec2(4.0f, 4.0f));
    }

protected:
    LibPngWrapper _libPng;
    std::unique_ptr<IEngine> _engine;
    ICamera2d* _camera;
    std::unique_ptr<ITileAtlas> _atlasColorTilesEmptyCenters4x4;
};

TEST_F(SpriteTests, Given3x3SpriteAtOrigin_Camera8x6FovCenteredAtOrigin_EntireSpriteIsVisible)
{
    auto spriteInstance = _atlasColorTilesEmptyCenters4x4->CreateSpriteInstance(0);
    spriteInstance->Position(glm::vec2(0.0f, 0.0f));
    spriteInstance->Rotation(0.0f);
    spriteInstance->Size(glm::vec2(3.0f, 3.0f));

    _camera->Center(glm::vec2(0.0f, 0.0f));

    // Set FoV so that 8 units are visible horizontally and due
    // to the aspect ration 6 units will be visible vertically
    _camera->FieldOfView(ICamera2d::Fov(
        -4.0f, 4.0f,
        -4.0f * (1 / AspectRatio), 4.0f * (1 / AspectRatio)));

    _engine->Render();

    const unsigned int ColumnCount = 8;
    const unsigned int RowCount = 6;
    ExpectTileColorGridOnScreen<ColumnCount, RowCount>(
        *(_engine->TakeScreenshot()),
        std::array<std::array<Color, ColumnCount>, RowCount>(
        { {
            // 1st row is all background color except for 3 cells right of center
            { ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow, ColorBackgroundUglyYellow,
            ColorWhite, ColorWhite, ColorWhite, ColorBackgroundUglyYellow },

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
