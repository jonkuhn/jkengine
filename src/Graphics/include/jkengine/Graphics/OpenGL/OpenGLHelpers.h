#include "IOpenGLWrapper.h"

#include <string>
#include <sstream>

namespace Graphics::OpenGL
{
    void ThrowIfOpenGlError(IOpenGLWrapper& gl, const std::string& msgPrefix);
}
