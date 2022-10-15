#include "IOpenGLWrapper.h"

#include <string>
#include <sstream>

namespace JkEng::Graphics::OpenGL
{
    void ThrowIfOpenGlError(IOpenGLWrapper& gl, const std::string& msgPrefix);
}
