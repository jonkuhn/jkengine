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

        void ClearColor(const Color &color) override
        {
            _gl.ClearColor(
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(color.a) / 255.0f
            );
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
        Registry<TileAtlas> _tileAtlasRegistry;
    };
}
