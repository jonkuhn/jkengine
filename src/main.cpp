#include <array>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include "Graphics/Graphics.h"
#include "Input/Input.h"

#include "Window/GlfwWindow.h"
#include "Window/GlfwWrapper.h"


// settings
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

void processInput(Window::GlfwWindow& window)
{
    if(window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        window.Close();
}

struct Vertex
{
    float x;
    float y;
    float z;
};

class RandomTileMap : public Graphics::IImage
{
public:
    RandomTileMap(int width, int height)
        : _width(width),
          _height(height),
          _data(4 * _width * _height)
    {
        for(int x = 0; x < _width; x++)
        {
            for(int y = 0; y < _height; y++)
            {
                _data[y * _width * 4 + x * 4 + 0] = std::rand() % 2;
                _data[y * _width * 4 + x * 4 + 1] = std::rand() % 2;
                _data[y * _width * 4 + x * 4 + 2] = 0;
                _data[y * _width * 4 + x * 4 + 3] = 0;
            }
        }
    }

    const uint8_t* Data() const
    {
        return _data.data();
    }

    int Width() const
    {
        return _width;
    }

    int Height() const
    {
        return _height;
    }

    PixelFormat Format() const
    {
        return IImage::PixelFormat::RGBA;
    }

private:
    int _width;
    int _height;
    std::vector<uint8_t> _data;
};

class TestTileMapImage : public Graphics::IImage
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

int main()
{
    Graphics::LibPngWrapper libpng;
    Window::GlfwWrapper glfw;
    Window::GlfwWindow window(glfw, SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL");

    Graphics::OpenGL::Engine oglGfxEng(window, 3);
    Graphics::IEngine& graphicsEngine = oglGfxEng;

    Input::Glfw::Engine glfwInpEng(glfw, window);
    Input::IEngine& inputEngine = glfwInpEng;

    const int catSpriteSheetWidth = 5;
    const int catSpriteSheetHeight = 5;
    Graphics::PngImage catTileAtlasImage(&libpng, "assets/CatAnimations32x32.png");
    auto catTileAtlas = graphicsEngine.CreateTileAtlas(
        catTileAtlasImage,
        glm::vec2(catSpriteSheetWidth, catSpriteSheetHeight)
    );

    auto catSprite = catTileAtlas->CreateSprite(1);
    catSprite->Position(glm::vec2(0.0f, 1.0f));
    catSprite->Size(glm::vec2(1.0f, 1.0f));

    const float catWalkVelocity = 0.375f * 2.0f;
    const float catRunVelocity = 3.2f;

    //auto aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    auto camera2d = graphicsEngine.Camera2d();
    camera2d->FieldOfView(Graphics::ICamera2d::Fov(
            -8.0f, 8.0f,
            -4.5f, 4.5f));
    camera2d->Center(glm::vec2(8.0f, 4.5f));
    auto catDX = 0.0f;
    float catAnimationCurrentFrameTime = 0.0f;
    int catAnimationFrameBase = 0;
    int catAnimationFrameOffset = 0;

    double previousTime = glfwGetTime();

    graphicsEngine.ClearColor(Graphics::Color(51, 77, 77, 255));

    //Input::IGamepad* playerGamepad = nullptr;

    constexpr int catRunAnimationStartFrame = 10;
    constexpr int catRunAnimationFrames = 8;
    //std::array<float, catRunAnimationFrames> runFrameTimes = { 0.0625, 0.125f, 0.125f, 0.0625f, 0.0625 };
    std::array<float, catRunAnimationFrames> runFrameTimes = { 0.0625, 0.0625f, 0.0625f, 0.0625f, 0.0625, 0.0625f, 0.0625f, 0.0625f };

    while (!window.WindowShouldClose())
    {
        inputEngine.Update();

        // TODO: move keyboard input into Input library at some point
        if (window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.Close();
        }

/*
        if (playerGamepad == nullptr)
        {
            std::cout << "Looking for player gamepad" << std::endl;
            for (auto& gamepad : inputEngine.EnumerateGamepads())
            {
                if (gamepad->IsConnected())
                {
                    std::cout << "Found Gamepad: " << gamepad->DisplayName() << std::endl;

                    if (playerGamepad == nullptr)
                    {
                        playerGamepad = gamepad;
                        std::cout << "Using " << gamepad->DisplayName() << " as player gamepad" << std::endl;
                    }
                }
            }
        }

        if(playerGamepad == nullptr || !playerGamepad->IsConnected())
        {
            std::cout << "Player gamepad is not connected!" << std::endl;
        }
        else
        {
            std::cout << " FR:" << playerGamepad->RightFaceButton();
            std::cout << " FL:" << playerGamepad->LeftFaceButton();
            std::cout << " FT:" << playerGamepad->TopFaceButton();
            std::cout << " FB:" << playerGamepad->BottomFaceButton();
            std::cout << " DD:" << playerGamepad->DPadDown();
            std::cout << " DU:" << playerGamepad->DPadUp();
            std::cout << " DL:" << playerGamepad->DPadLeft();
            std::cout << " DR:" << playerGamepad->DPadRight();
            std::cout << " LB:" << playerGamepad->LeftBumperButton();
            std::cout << " RB:" << playerGamepad->RightBumperButton();
            std::cout << " TL: " << playerGamepad->LeftTriggerButton();
            std::cout << " TR: " << playerGamepad->RightTriggerButton();
            std::cout << " LSTK:" << playerGamepad->LeftStickButton();
            std::cout << " RSTK:" << playerGamepad->RightStickButton();
            std::cout << " SELECT:" << playerGamepad->SelectButton();
            std::cout << " START:" << playerGamepad->StartButton();
        
            std::cout << " LX: " << playerGamepad->LeftStickX();
            std::cout << " LY: " << playerGamepad->LeftStickY();
            std::cout << " RX: " << playerGamepad->RightStickX();
            std::cout << " RY: " << playerGamepad->RightStickY();
            std::cout << std::endl;
        }
        */

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        bool catRunning = false;
        int catDirection;
        if(window.GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            catDX = catWalkVelocity;
            catDirection = 1;

        }
        else if(window.GetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            catDX = -catWalkVelocity;
            catDirection = -1;

        }
        else if(window.GetKey(GLFW_KEY_D) == GLFW_PRESS)
        {
            catDX = catRunVelocity;
            catDirection = 1;
            catRunning = true;
        }
        else if(window.GetKey(GLFW_KEY_A) == GLFW_PRESS)
        {
            catDX = -catRunVelocity;
            catDirection = -1;
            catRunning = true;
        }
        else
        {
            catDX = 0;
            catDirection = 0;
        }

        catAnimationCurrentFrameTime += deltaTime;
        if (!catRunning && catAnimationCurrentFrameTime >= (0.08333f/2.0f))
        {
            catAnimationFrameBase = 0;
            catAnimationFrameOffset = (8 + catAnimationFrameOffset + catDirection) % 8;
            catAnimationCurrentFrameTime = 0.0f;
        }
        else if (catRunning && catAnimationFrameBase < catRunAnimationStartFrame)
        {
            catAnimationFrameBase = catRunAnimationStartFrame;
            catAnimationFrameOffset = 0;
            catAnimationCurrentFrameTime = 0.0f;
        }
        else if (catRunning && catAnimationCurrentFrameTime >= runFrameTimes[catAnimationFrameOffset])
        {
            catAnimationFrameBase = catRunAnimationStartFrame;
            catAnimationFrameOffset = (catRunAnimationFrames + catAnimationFrameOffset + catDirection) % catRunAnimationFrames;
            catAnimationCurrentFrameTime = 0.0f;
        }

        catSprite->AtlasLocation(Graphics::GridLocation(
            (catAnimationFrameBase + catAnimationFrameOffset) % catSpriteSheetWidth,
            (catAnimationFrameBase + catAnimationFrameOffset) / catSpriteSheetWidth));

        auto catPos = glm::vec2(catSprite->Position());
        catPos.x += catDX * deltaTime;
        catSprite->Position(catPos);

        graphicsEngine.Render();
    }

    return 0;
}
