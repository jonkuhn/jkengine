#pragma once

#include <memory>

#include "../IEngine.h"
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
            IImage& tileAtlas,
            const glm::vec2& atlasSizeInTiles) override;

    private:
        GlfwWrapper _glfw;
        GlfwWindow _window;
        OpenGLWrapper _gl;
        TileMapShaderProgram _tileMapShaderProgram;
        UnitQuadVertexArray _unitQuadVertexArray;

        std::vector<std::unique_ptr<Texture>> _textures;

        Texture* CreateTexture(const Texture::Params &params);
    };
}
