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

#include "Graphics/LibPngWrapper.h"
#include "Graphics/PngImage.h"
#include "Graphics/OpenGL/Engine.h"

using namespace Graphics;
using namespace Graphics::OpenGL;

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

void processInput(GlfwWindow& window)
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

class RandomTileMap : public IImage
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

int main()
{
    LibPngWrapper libpng;
    Engine engine(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL");

    // Want:
    // - World to be 100 tiles by 100 tiles represented by one tile map
    // - View to be 10 tiles by 10 tiles
    // - Each tile to be 64 by 64 pixels
    // - So world is 64,000 by 64,000
    // - So view is 640 by 640
    const float WORLD_WIDTH_IN_TILES = 100;
    const float WORLD_HEIGHT_IN_TILES = 100; 

    // Define a model matrix that scale's up from a unit quad
    // to world width by world height
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
    model = glm::rotate(model, glm::radians(12.5f), glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES, 0.0f));  

    // Generate a random tile map texture using a helper class
    RandomTileMap randomTileMap(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES);
    //Texture tileMapTexture(&gl, Texture::Params(randomTileMap)
    //    .WrapModeS(Texture::WrapMode::ClampToBorder)
    //    .WrapModeT(Texture::WrapMode::ClampToBorder)
    //    .MinFilter(Texture::MinFilterMode::Nearest)
    //    .MagFilter(Texture::MagFilterMode::Nearest));

    // use a small 2x2 tile atlas for testing purposes
    const float TILE_ATLAS_WIDTH_IN_TILES = 2;
    const float TILE_ATLAS_HEIGHT_IN_TILES = 2;
    PngImage tileAtlasImage(&libpng, "TestFiles/scribbletiles.png");
    //Texture tileAtlasTexture(&gl, Texture::Params(tileAtlasImage)
    //    .WrapModeS(Texture::WrapMode::ClampToBorder)
    //    .WrapModeT(Texture::WrapMode::ClampToBorder)
    //    .MinFilter(Texture::MinFilterMode::Nearest)
    //    .MagFilter(Texture::MagFilterMode::Nearest));

    auto tileMap = engine.CreateTileMap(
        randomTileMap,
        glm::vec2(WORLD_WIDTH_IN_TILES, WORLD_HEIGHT_IN_TILES),
        tileAtlasImage,
        glm::vec2(TILE_ATLAS_WIDTH_IN_TILES, TILE_ATLAS_HEIGHT_IN_TILES)
    );

    const float MOVE_SPEED = 5.0f;

    auto cameraX = 0.0f;
    auto cameraY = 0.0f;
    auto cameraDX = 0.0f;
    auto cameraDY = 0.0f;

    double previousTime = glfwGetTime();

    auto& window = engine.GetWindow();
    while (window.Update())
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //tileMapShaderProgram.Use();
        
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

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

        cameraX += cameraDX * deltaTime;
        cameraY += cameraDY * deltaTime;

        // 2D Camera
        // Notes:
        // - Z component of cameraPosition must be such that the objects
        //   that should be visible are within the range of zNear to zFar
        //   from the camera.  (where zNear and zFar are the arguments to
        //   glm::ortho).  If zNear is negative objects behind the camera
        //   will be rendered.
        // - In order for the center argument to lookAt (cameraPos + cameraFront)
        //   to truly be centered in the window, the left and right and
        //   top and bottom arguments to glm::ortho must be set such that
        //   0 is the midpoint (i.e. left = -right bottom = -top)
        // - cameraFront is a normalized vector that represents the direction
        //   the camera is pointing
        // - cameraUp is a normalized vector that points to the up direction
        //   in world space.  glm::lookAt uses this together with "center"
        //   (the direction the camera is pointing -- cameraPos + cameraFront)
        //   to get a vector that points to the right in view space.  This is
        //   done by taking the cross product of the two to get an orthogonal
        //   vector.  glm::lookAt then takes the cross product of resulting
        //   "right" vector and the vector representing the direction the
        //   camera is pointing to get the "up" vector which is a vector
        //   pointing out of the top of the camera.  The actual matrix
        //   returned by glm::lookAt is built from the "direction", "right",
        //   and "up" vectors (note "up" != cameraUp)
        //   (See https://learnopengl.com/Getting-started/Camera)
        glm::vec3 cameraPos   = glm::vec3(cameraX, cameraY, 0.5f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        auto aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        //auto fovWidthInWorldCoordinates = 5.0f;
        auto projection = glm::ortho(
            -2.5f * aspectRatio,
            2.5f * aspectRatio,
            -2.5f,
            2.5f,
            0.0f,
            1.0f);

        tileMap->Draw(model, view, projection);
    }

    return 0;
}
