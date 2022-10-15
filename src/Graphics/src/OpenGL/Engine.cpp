#include "OpenGL/Engine.h"

#include "SceneDefinition.h"

#include "OpenGL/Scene.h"
#include "OpenGL/Texture.h"
#include "OpenGL/ViewportCapture.h"

using namespace JkEng::Graphics::OpenGL;

Engine::Engine(JkEng::Window::IOpenGLWindow& window)
    : _window(window),
      _gl(_window)
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

std::unique_ptr<JkEng::Graphics::IScene> Engine::CreateScene(const SceneDefinition& definition)
{
    return std::make_unique<Scene>(_window, definition);
}

std::unique_ptr<JkEng::Graphics::IScreenshot> Engine::TakeScreenshot()
{
    return std::make_unique<ViewportCapture>(_gl);
}
