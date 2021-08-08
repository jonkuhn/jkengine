#include "Engine.h"

#include "Texture.h"
#include "ViewportCapture.h"

using namespace Graphics::OpenGL;

Engine::Engine(int winWidth, int winHeight, const std::string& title, unsigned int numberOfDrawingLayers)
    : _glfw(),
      _window(&_glfw, winWidth, winHeight, title),
      _gl(&_window),
      _tileMapShaderProgram(_gl),
      _spriteShaderProgram(_gl),
      _unitQuadVertexArray(_gl),
      _camera2d(),
      _tileMapDrawer(_tileMapShaderProgram, _unitQuadVertexArray, _camera2d),
      _spriteDrawer(_spriteShaderProgram, _unitQuadVertexArray, _camera2d),
      _tileAtlasRegistry(),
      _numberOfDrawingLayers(numberOfDrawingLayers),
      _programShouldExit(false)
{
    // Enable alpha blending so that sprites and tile maps can use
    // transparency.  Set the blend function to use the source alpha
    // to determine the weight to be given to the source (new fragment
    // color) and one minus the source alpha determine the weight to
    // be given to the current color of the fragment (the color it
    // got from whatever was already drawn there).
    //
    // Because alpha blending and depth testing do not work well
    // together the engine is initialized with a number of drawing
    // layers and then each object is associated with a single drawing
    // layer.  These layers are then drawn back to front.  Anything
    // that needs to appear in front of something else should be placed
    // on a higher numbered layer.
    _gl.Enable(GL_BLEND);
    _gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::unique_ptr<Graphics::ITileAtlas> Engine::CreateTileAtlas(
    const IImage& tileAtlasImage,
    const glm::vec2& atlasSizeInTiles)
{
    return std::make_unique<TileAtlas>(
        _tileAtlasRegistry,
        _gl,
        _numberOfDrawingLayers,
        &_tileMapDrawer,
        &_spriteDrawer,
        Texture(
            _gl,
            Texture::Params(tileAtlasImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        atlasSizeInTiles);
}

std::unique_ptr<Graphics::IScreenshot> Engine::TakeScreenshot()
{
    return std::make_unique<ViewportCapture>(_gl);
}

void Engine::Render()
{
    _gl.Clear(GL_COLOR_BUFFER_BIT);

    for(unsigned int layer = 0; layer < _numberOfDrawingLayers; layer++)
    {
        for(auto* tileAtlas : _tileAtlasRegistry)
        {
            tileAtlas->DrawAllOnLayer(layer);
        }
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
