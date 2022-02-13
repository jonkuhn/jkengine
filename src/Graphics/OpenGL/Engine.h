#pragma once

#include <memory>

#include <Shared/Registry.h>
#include <Window/IOpenGLWindow.h>

#include "../IEngine.h"
#include "Camera2d.h"
#include "OpenGLWrapper.h"
#include "ShaderProgram.h"
#include "SpriteDrawer.h"
#include "SpriteShaderProgram.h"
#include "Texture.h"
#include "TileAtlas.h"
#include "TileMapDrawer.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"
#include "VertexArray.h"

namespace Graphics::OpenGL
{
    class Engine : public IEngine
    {
    public:
        Engine(Window::IOpenGLWindow& window, unsigned int numberOfDrawingLayers);

        // There is need to copy or move this class.
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

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

        void Render() override;

    private:
        Window::IOpenGLWindow& _window;
        OpenGLWrapper _gl;
        TileMapShaderProgram _tileMapShaderProgram;
        SpriteShaderProgram _spriteShaderProgram;
        UnitQuadVertexArray _unitQuadVertexArray;
        class Camera2d _camera2d;
        TileMapDrawer _tileMapDrawer;
        SpriteDrawer _spriteDrawer;
        Shared::Registry<TileAtlas> _tileAtlasRegistry;

        unsigned int _numberOfDrawingLayers;
    };
}
