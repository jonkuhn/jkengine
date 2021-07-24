#include "Shader.h"

#include <string>
#include <sstream>
#include <vector>

#include "IOpenGLWrapper.h"

using namespace Graphics::OpenGL;

namespace 
{
    using namespace Graphics::OpenGL;

    std::string readAllToString(std::istream& istream)
    {
        std::istreambuf_iterator<char> eos;
        return std::string(std::istreambuf_iterator<char>(istream), eos);
    }

    GLenum typeToShaderEnum(Shader::Type type)
    {
        switch (type)
        {
            case Shader::Type::Vertex: return GL_VERTEX_SHADER;
            case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
            default:
            {
                std::stringstream ss;
                ss << "Invalid shader type: " << static_cast<int>(type);
                throw std::invalid_argument(ss.str().c_str());
            }
        }
    }
}

Shader::Shader(IOpenGLWrapper* gl, Type type, std::istream& sourceCodeStream)
    : Shader(gl, type, readAllToString(sourceCodeStream))
{

}

Shader::Shader(IOpenGLWrapper* gl, Type type, const std::string& source)
    : _gl(gl),
      _handle(
          _gl,
          _gl->CreateShader(typeToShaderEnum(type)),
          [](IOpenGLWrapper* gl, GLuint h){ gl->DeleteShader(h); })
{
    const char* sourceCstr = source.c_str();

    if (!_handle.get())
    {
        std::stringstream ss;
        ss << "glCreateShader failed with error: " << _gl->GetError();
        throw std::runtime_error(ss.str().c_str());
    }

    _gl->ShaderSource(_handle.get(), 1, &sourceCstr, nullptr);
    _gl->CompileShader(_handle.get());

    int success = 0;
    _gl->GetShaderiv(_handle.get(), GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint infoLogLength;
        _gl->GetShaderiv(_handle.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        _gl->GetShaderInfoLog(_handle.get(), infoLog.size(), NULL, infoLog.data());
        std::stringstream ss;
        ss << "Shader Compilation Failed: " << infoLog.data() << "" << std::endl
            << "Shader Source:" << std::endl
            << source << std::endl;
        throw std::runtime_error(ss.str().c_str());
    }
}

GLuint Shader::Handle()
{
    return _handle.get();
}
