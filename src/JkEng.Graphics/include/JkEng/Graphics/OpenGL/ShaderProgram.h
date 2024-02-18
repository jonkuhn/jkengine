#pragma once

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IShader.h"
#include "UniqueHandle.h"

namespace JkEng::Graphics::OpenGL
{
    class IOpenGLWrapper;

    class ShaderProgram final
    {
    public:
        ShaderProgram(IOpenGLWrapper& gl, const IShader& vertexShader, const IShader& fragmentShader);

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = default;
        ShaderProgram& operator=(ShaderProgram&& ) = default;


        void Use();
        void SetUniform(const std::string& name, int value);
        void SetUniform(const std::string& name, const glm::mat4& value);
        void SetUniform(const std::string& name, const glm::vec3& value);
        void SetUniform(const std::string& name, const glm::vec2& value);

    private:
        IOpenGLWrapper* _gl;

        typedef UniqueHandle<std::function<void (IOpenGLWrapper&, GLuint)>> UniqueProgramHandle;
        UniqueProgramHandle _handle;
    };
}
