#include "OpenGL/Scene.h"

#include <jkengine/Window/IOpenGLWindow.h>

#include "Color.h"
#include "SceneDefinition.h"

#include "OpenGL/Texture.h"

using namespace Graphics::OpenGL;

Scene::Scene(Window::IOpenGLWindow& window, const SceneDefinition& definition)
    : _window(window),
      _gl(_window),
      _tileMapShaderProgram(_gl),
      _spriteShaderProgram(_gl),
      _unitQuadVertexArray(_gl),
      _camera2d(),
      _tileMapDrawer(_tileMapShaderProgram, _unitQuadVertexArray, _camera2d),
      _spriteDrawer(_spriteShaderProgram, _unitQuadVertexArray, _camera2d),
      _tileAtlases(),
      _numberOfDrawingLayers(definition.NumberOfDrawingLayers())
{
    // Enable alpha blending so that sprites and tile maps can use
    // transparency.  Set the blend function to use the source alpha
    // to determine the weight to be given to the source (new fragment
    // color) and one minus the source alpha determine the weight to
    // be given to the current color of the fragment (the color it
    // got from whatever was already drawn there).
    //
    // Because alpha blending and depth testing do not work well
    // together the scene is initialized with a number of drawing
    // layers and then each object is associated with a single drawing
    // layer.  These layers are then drawn back to front.  Anything
    // that needs to appear in front of something else should be placed
    // on a higher numbered layer.
    _gl.Enable(GL_BLEND);
    _gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _tileAtlases.reserve(definition.TileAtlasDefinitions().size());
    for (auto& tileAtlasDefinition : definition.TileAtlasDefinitions())
    {
        _tileAtlases.emplace_back(
            _gl,
            _tileMapDrawer,
            _spriteDrawer,
            tileAtlasDefinition);
    }
}

void Scene::ClearColor(const Color &color)
{
    _gl.ClearColor(
        static_cast<float>(color.r) / 255.0f,
        static_cast<float>(color.g) / 255.0f,
        static_cast<float>(color.b) / 255.0f,
        static_cast<float>(color.a) / 255.0f
    );
}

void Scene::Render()
{
    _gl.Clear(GL_COLOR_BUFFER_BIT);

    for(unsigned int layer = 0; layer < _numberOfDrawingLayers; layer++)
    {
        for (auto& tileAtlas : _tileAtlases)
        {
            tileAtlas.DrawAllOnLayer(layer);
        }
    }

    _window.Update();
}
