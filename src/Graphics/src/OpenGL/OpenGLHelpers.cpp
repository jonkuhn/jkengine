#include "OpenGL/OpenGLHelpers.h"

void Graphics::OpenGL::ThrowIfOpenGlError(IOpenGLWrapper& gl, const std::string& msgPrefix)
{
    if (gl.GetError() != GL_NO_ERROR)
    {
        std::stringstream ss;
        ss << msgPrefix << " failed with error: " << gl.GetError();
        throw std::runtime_error(ss.str().c_str());
    }
}