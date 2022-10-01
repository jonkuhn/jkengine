#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>

#include "../IScene.h"
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

namespace Window
{
    class IOpenGLWindow;
}

namespace Graphics
{
    class SceneDefinition;
}

namespace Graphics::OpenGL
{
    class Scene : public IScene
    {
    public:

        Scene(Window::IOpenGLWindow& window, const SceneDefinition& definition);

        // There is need to copy or move this class.
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(Scene&&) = delete;

        void ClearColor(const Color &color) override;

        ICamera2d* Camera2d() override
        {
            return &_camera2d;
        }

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
        std::vector<TileAtlas> _tileAtlases;
        size_t _numberOfDrawingLayers;
    };
}
