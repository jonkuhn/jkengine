#include "Engine.h"
#include "Texture.h"
#include "ViewportCapture.h"

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
    const IImage& tileAtlasImage,
    const glm::vec2& atlasSizeInTiles)
{
    return std::make_unique<TileAtlas>(
        &_gl,
        &_tileMapShaderProgram,
        &_unitQuadVertexArray,
        &_camera2d,
        Texture(
            &_gl,
            Texture::Params(tileAtlasImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        atlasSizeInTiles);
}

std::unique_ptr<Graphics::IScreenshot> Engine::TakeScreenshot()
{
    return std::make_unique<ViewportCapture>(&_gl);
}

void Engine::Render()
{
    // TODO: Loop over everything and draw it

    // TODO: Window should probably exist outside of Graphics namespace
    // Because the program needing to exit is really an input concern
    if(!_window.Update())
    {
        _programShouldExit = true;
    }
}
