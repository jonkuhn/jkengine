#include "OpenGL/ShaderProgram.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma clang diagnostic pop

#include <string>
#include <sstream>
#include <vector>

#include "OpenGL/IOpenGLWrapper.h"

using namespace JkEng::Graphics::OpenGL;

ShaderProgram::ShaderProgram(IOpenGLWrapper& gl, const IShader& vertexShader, const IShader& fragmentShader)
    : _gl(&gl),
      _handle(
          *_gl,
          _gl->CreateProgram(),
          [](IOpenGLWrapper& gl, GLuint h) {gl.DeleteProgram(h); })
{
    if (!_handle.get())
    {
        std::stringstream ss;
        ss << "glCreateProgram failed with error: " << _gl->GetError();
        throw std::runtime_error(ss.str().c_str());
    }

    _gl->AttachShader(_handle.get(), vertexShader.Handle());
    _gl->AttachShader(_handle.get(), fragmentShader.Handle());

    _gl->LinkProgram(_handle.get());

    // check for linking errors
    GLint success = 0;
    _gl->GetProgramiv(_handle.get(), GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLogLength;
        _gl->GetProgramiv(_handle.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        _gl->GetProgramInfoLog(_handle.get(), infoLog.size(), NULL, infoLog.data());
        std::stringstream ss;
        ss << "Shader Program Linking Failed: " << infoLog.data() << "" << std::endl;
        throw std::runtime_error(ss.str().c_str());
    }

    _gl->DetachShader(_handle.get(), vertexShader.Handle());
    _gl->DetachShader(_handle.get(), fragmentShader.Handle());
}

void ShaderProgram::Use()
{
    _gl->UseProgram(_handle.get());
}

void ShaderProgram::SetUniform(const std::string& name, int value)
{
    // First, ensure this is the program in use, so the caller doesn't
    // have to call Use() before calling SetUniform.
    Use();
    _gl->Uniform1i(_gl->GetUniformLocation(_handle.get(), name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
{
    // First, ensure this is the program in use, so the caller doesn't
    // have to call Use() before calling SetUniform.
    Use();
    _gl->UniformMatrix4fv(
        _gl->GetUniformLocation(_handle.get(), name.c_str()),
        1,
        false,
        glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
{
    // First, ensure this is the program in use, so the caller doesn't
    // have to call Use() before calling SetUniform.
    Use();
    _gl->Uniform3fv(
        _gl->GetUniformLocation(_handle.get(), name.c_str()),
        1,
        glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& value)
{
    // First, ensure this is the program in use, so the caller doesn't
    // have to call Use() before calling SetUniform.
    Use();
    _gl->Uniform2fv(
        _gl->GetUniformLocation(_handle.get(), name.c_str()),
        1,
        glm::value_ptr(value));
}
