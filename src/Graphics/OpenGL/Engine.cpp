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

std::unique_ptr<Graphics::ITileMap> Engine::CreateTileMap(
    IImage& tileMapImage,
    const glm::vec2& mapSizeInTiles,
    IImage& tileAtlasImage,
    const glm::vec2& atlasSizeInTiles)
{
    auto tileMapUniquePtr = std::make_unique<TileMap>(
        &_tileMapShaderProgram,
        &_unitQuadVertexArray,
        Texture(
            &_gl,
            Texture::Params(tileMapImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        mapSizeInTiles,
        Texture(
            &_gl,
            Texture::Params(tileAtlasImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        atlasSizeInTiles);
    return tileMapUniquePtr;
}
