#include "OpenGL/OpenGLHelpers.h"

void JkEng::Graphics::OpenGL::ThrowIfOpenGlError(IOpenGLWrapper& gl, const std::string& msgPrefix)
{
    GLenum glError = gl.GetError();
    if (glError != GL_NO_ERROR)
    {
        std::stringstream ss;
        ss << msgPrefix << " failed with error: " << glError;
        throw std::runtime_error(ss.str().c_str());
    }
}