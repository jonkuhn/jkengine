#if 0
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return 1;
    }

    // SWAP A and B on Xbox One controller (works)
    //if (glfwUpdateGamepadMappings("030000005e040000fd02000003090000,Xbox Wireless Controller,a:b1,b:b0,back:b16,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b15,leftshoulder:b6,leftstick:b13,lefttrigger:a5,leftx:a0,lefty:a1,rightshoulder:b7,rightstick:b14,righttrigger:a4,rightx:a2,righty:a3,start:b11,x:b3,y:b4,platform:Mac OS X,\r\n") == GLFW_FALSE)
    //if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),leftx:h0.2,lefty:h0.4,leftx:h0.8,lefty:h0.1,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)
    if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)
    {
        std::cout << "glfwUpdateGamepadMappings failed" << std::endl;
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        std::cout << "glfwCreateWindow failed" << std::endl;
        return 1;
    }

    double lastScanTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        double currentTime = glfwGetTime();
        if(lastScanTime < (currentTime - 1.0))
        {
            lastScanTime = currentTime;
            std::cout << "Scanning for joysticks..." << std::endl;
            for(int j = GLFW_JOYSTICK_1; j <= GLFW_JOYSTICK_LAST; j++)
            {
                auto* name = glfwGetJoystickName(j);
                if (name != nullptr)
                {
                    auto* guid = glfwGetJoystickGUID(j);
                    bool isGamepad = (glfwJoystickIsGamepad(j) == GLFW_TRUE);

                    //bool a = false;
                    //bool b = false;
                    if (isGamepad)
                    {
                        GLFWgamepadstate gamepadState;
                        if(glfwGetGamepadState(j, &gamepadState) == GLFW_FALSE)
                        {
                            std::cout << "glfwGetGamepadState failed" << std::endl;
                        }
                        //a = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A];
                        //b = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_B];
                    }

                    int count = 0;
                    const float* axes = glfwGetJoystickAxes(j, &count);

                    for(int a = 0; a < count; a++)
                    {
                        std::cout << "a" << a << " = " << *(axes + a) << ", ";
                    }
                    std::cout << std::endl;

                    std::cout << name << " GUID: " << guid << " is gamepad: " << isGamepad << std::endl;
                    //std::cout << name << " GUID: " << guid << " is gamepad: " << isGamepad
                    //    << " a: " << a << " b: " << b << std::endl;

                }
            }
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
#endif

#include <array>
#include <cstdlib>
#include <fstream>
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
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

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

    // Want:
    // - World to be 100 tiles by 100 tiles represented by one tile map
    // - View to be 10 tiles by 10 tiles
    // - Each tile to be 64 by 64 pixels
    // - So world is 64,000 by 64,000
    // - So view is 640 by 640
    const float WORLD_WIDTH_IN_TILES = 8;
    const float WORLD_HEIGHT_IN_TILES = 4; 

    // Generate a random tile map texture using a helper class
    //RandomTileMap randomTileMap(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES);
    TestTileMapImage randomTileMap;
    //Texture tileMapTexture(&gl, Texture::Params(randomTileMap)
    //    .WrapModeS(Texture::WrapMode::ClampToBorder)
    //    .WrapModeT(Texture::WrapMode::ClampToBorder)
    //    .MinFilter(Texture::MinFilterMode::Nearest)
    //    .MagFilter(Texture::MagFilterMode::Nearest));

    // use a small 2x2 tile atlas for testing purposes
    const float TILE_ATLAS_WIDTH_IN_TILES = 4;
    const float TILE_ATLAS_HEIGHT_IN_TILES = 4;
    Graphics::PngImage tileAtlasImage(&libpng, "TestFiles/colortiles4x4emptycenters.png");
    //Texture tileAtlasTexture(&gl, Texture::Params(tileAtlasImage)
    //    .WrapModeS(Texture::WrapMode::ClampToBorder)
    //    .WrapModeT(Texture::WrapMode::ClampToBorder)
    //    .MinFilter(Texture::MinFilterMode::Nearest)
    //    .MagFilter(Texture::MagFilterMode::Nearest));

    auto tileAtlas = graphicsEngine.CreateTileAtlas(
        tileAtlasImage,
        glm::vec2(TILE_ATLAS_WIDTH_IN_TILES, TILE_ATLAS_HEIGHT_IN_TILES)
    );


    // Define a model matrix that scale's up from a unit quad
    // to world width by world height
    auto tileMap1 = tileAtlas->CreateTileMap(1, randomTileMap);
    tileMap1->Position(glm::vec2(-2.0f, -2.0f));
    tileMap1->Rotation(12.5f);
    tileMap1->Size(glm::vec2(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES));

    auto tileMap2 = tileAtlas->CreateTileMap(0, randomTileMap);
    tileMap2->Position(glm::vec2(-2.0f, -2.0f));
    tileMap2->Size(glm::vec2(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES));

    const float MOVE_SPEED = 5.0f;

    auto camera2d = graphicsEngine.Camera2d();
    auto aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    camera2d->FieldOfView(Graphics::ICamera2d::Fov(
            -2.5f * aspectRatio, 2.5f * aspectRatio,
            -2.5f, 2.5f));
    auto cameraDX = 0.0f;
    auto cameraDY = 0.0f;

    double previousTime = glfwGetTime();

    graphicsEngine.ClearColor(Graphics::Color(51, 77, 77, 255));

    Input::IGamepad* playerGamepad = nullptr;

/*
    std::ifstream ifs("gamecontrollerdb.txt", std::ifstream::binary);
    if (!ifs)
    {
        std::cout << "open failed" << std::endl;
        return 0;
    }

    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    std::vector<char> buffer(length+1);
    ifs.seekg(0, ifs.beg);
    ifs.read(&buffer[0], length);
    buffer[length] = '\0';
    ifs.close();

    std::cout << "length of mappings: " << length << std::endl;
*/

    // The reason the following cannot be loaded is because GLFW does not support output modifiers
    // (the + and - before leftx and left y)
    //if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),+leftx:h0.2,+lefty:h0.4,-leftx:h0.8,-lefty:h0.1,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)

    // This doesn't work entirely right because since output modifiers are not supported it only
    // maps half of the left axes.
    //if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),leftx:h0.2,lefty:h0.4,leftx:h0.8,lefty:h0.1,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)

    // This works but is not analog because the joystick "hat" states are binary.
    // Even with the above mappings with the modifiers it was really just mapping
    // binary hat states to each half of each axis.
    if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)
    {
        std::cout << "error" << std::endl;
        const char* errorDescription;
        auto errorCode = glfwGetError(&errorDescription);
        std::stringstream ss;
        ss << "Failed to update gamepad mappings: error code:"
        << errorCode << " desc: " << errorDescription;
        throw std::runtime_error(ss.str());
    }

    //double lastScanTime = 0;
    while (!window.WindowShouldClose())
    {
        inputEngine.Update();

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

/*
        if(lastScanTime < (currentTime - 1.0))
        {
            lastScanTime = currentTime;
            if (glfwUpdateGamepadMappings("030000007e0500000620000001000000,Joy-Con (L),+leftx:h0.2,+lefty:h0.4,-leftx:h0.8,-lefty:h0.1,a:b0,b:b1,back:b13,leftshoulder:b4,leftstick:b10,rightshoulder:b5,start:b8,x:b2,y:b3,platform:Mac OS X,\r\n") == GLFW_FALSE)
            {
                std::cout << "error" << std::endl;
                const char* errorDescription;
                auto errorCode = glfwGetError(&errorDescription);
                std::stringstream ss;
                ss << "Failed to update gamepad mappings: error code:"
                << errorCode << " desc: " << errorDescription;
                throw std::runtime_error(ss.str());
            }

            const char* errorDescription = nullptr;
            auto errorCode = glfwGetError(&errorDescription);
            std::cout << "error code:" << errorCode << std::endl;

            std::cout << "Scanning for joysticks..." << std::endl;
            for(int j = GLFW_JOYSTICK_1; j <= GLFW_JOYSTICK_LAST; j++)
            {
                auto* name = glfwGetJoystickName(j);
                if (name != nullptr)
                {
                    auto* guid = glfwGetJoystickGUID(j);
                    bool isGamepad = (glfwJoystickIsGamepad(j) == GLFW_TRUE);

                    std::cout << name << " GUID: " << guid << " is gamepad: " << isGamepad << std::endl;
                }
            }
        }
*/

        // TODO: move keyboard input into Input library at some point
        if (window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.Close();
        }

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

        if(window.GetKey(GLFW_KEY_UP) == GLFW_PRESS)
        {
            cameraDY = MOVE_SPEED;
        }
        else if(window.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            cameraDY = -MOVE_SPEED;
        }
        else
        {
            cameraDY = 0;
        }

        if(window.GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            cameraDX = MOVE_SPEED;
        }
        else if(window.GetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            cameraDX = -MOVE_SPEED;
        }
        else
        {
            cameraDX = 0;
        }

        //if (cameraDX != 0 && cameraDY != 0)
        //{
        //    cameraDX = (cameraDX / cameraDX) * std::sqrt(MOVE_SPEED);
        //    cameraDY = (cameraDY / cameraDY) * std::sqrt(MOVE_SPEED);
        //}

        //if (cameraX <= 0 && cameraDX < 0)
        //{
        //    cameraX = 0;
        //    cameraDX = 0;
        //}

        //if (cameraX >= (WORLD_WIDTH_IN_TILES - VIEW_WIDTH_IN_TILES) && cameraDX > 0)
        //{
        //    cameraX = WORLD_WIDTH_IN_TILES - VIEW_WIDTH_IN_TILES;
        //    cameraDX = -cameraDX;
        //}

        //if (cameraY <= 0 && cameraDY < 0)
        //{
        //    cameraY = 0;
        //    cameraDY = 0;
        //}

        //if (cameraY >= (WORLD_HEIGHT_IN_TILES - VIEW_HEIGHT_IN_TILES) && cameraDY > 0)
        //{
        //    cameraY = WORLD_HEIGHT_IN_TILES - VIEW_HEIGHT_IN_TILES;
        //    cameraDY = 0;
        //}

        auto cameraCenter = glm::vec2(camera2d->Center());
        cameraCenter.x += cameraDX * deltaTime;
        cameraCenter.y += cameraDY * deltaTime;
        camera2d->Center(cameraCenter);

        graphicsEngine.Render();
    }

    return 0;
}
