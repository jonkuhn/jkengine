#include "Engine.h"

#include "Texture.h"
#include "ViewportCapture.h"

using namespace Graphics::OpenGL;

Engine::Engine(int winWidth, int winHeight, const std::string& title)
    : _glfw(),
      _window(&_glfw, winWidth, winHeight, title),
      _gl(&_window),
      _tileMapShaderProgram(&_gl),
      _unitQuadVertexArray(&_gl),
      _camera2d(),
      _programShouldExit(false),
      _tileAtlasRegistry()
{

}

std::unique_ptr<Graphics::ITileAtlas> Engine::CreateTileAtlas(
    const IImage& tileAtlasImage,
    const glm::vec2& atlasSizeInTiles)
{
    return std::make_unique<TileAtlas>(
        &_tileAtlasRegistry,
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
    _gl.Clear(GL_COLOR_BUFFER_BIT);

    for(auto* tileAtlas : _tileAtlasRegistry)
    {
        tileAtlas->DrawAll();
    }

    // TODO: Window should probably exist outside of Graphics namespace
    // Because the program needing to exit is really an input concern.
    // (Update is still a graphics concern, but the return value from it
    // indicating if the program should exit really should be a different
    // function that is an input concern)
    if(!_window.Update())
    {
        _programShouldExit = true;
    }
}
