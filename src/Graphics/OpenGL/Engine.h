#pragma once

#include <memory>

#include "../IEngine.h"
#include "Camera2d.h"
#include "GlfwWrapper.h"
#include "OpenGLWrapper.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "TileAtlas.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"
#include "VertexArray.h"
#include "Window.h"

namespace Graphics::OpenGL
{
    class Engine : public IEngine
    {
    public:
        Engine(int winWidth, int winHeight, const std::string& title);

        GlfwWindow& GetWindow()
        {
            return _window;
        }

        std::unique_ptr<Graphics::ITileAtlas> CreateTileAtlas(
            const IImage& tileAtlas,
            const glm::vec2& atlasSizeInTiles) override;


        ICamera2d* Camera2d() override
        {
            return &_camera2d;
        }

        std::unique_ptr<IScreenshot> TakeScreenshot() override;

        // TODO: Window should probably exist outside of Graphics namespace
        // Because this is really and input concern.
        bool ProgramShouldExit() override
        {
            return _programShouldExit;
        }

        void Render() override;

    private:
        GlfwWrapper _glfw;
        GlfwWindow _window;
        OpenGLWrapper _gl;
        TileMapShaderProgram _tileMapShaderProgram;
        UnitQuadVertexArray _unitQuadVertexArray;
        class Camera2d _camera2d;
        bool _programShouldExit;
    };
}
