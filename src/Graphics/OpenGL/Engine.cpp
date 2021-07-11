#include "Engine.h"
#include "Texture.h"

using namespace Graphics::OpenGL;

Engine::Engine(int winWidth, int winHeight, const std::string& title)
    : _glfw(),
      _window(&_glfw, winWidth, winHeight, title),
      _gl(&_window),
      _tileMapShaderProgram(&_gl),
      _unitQuadVertexArray(&_gl)
{

}

std::unique_ptr<Graphics::ITileAtlas> Engine::CreateTileAtlas(
    IImage& tileAtlasImage,
    const glm::vec2& atlasSizeInTiles)
{
    return std::make_unique<TileAtlas>(
        &_gl,
        &_tileMapShaderProgram,
        &_unitQuadVertexArray,
        Texture(
            &_gl,
            Texture::Params(tileAtlasImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        atlasSizeInTiles);
}
