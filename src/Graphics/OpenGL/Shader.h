#pragma once
#include <functional>
#include <istream>

#include "IShader.h"
#include "UniqueHandle.h"

namespace Graphics::OpenGL
{
    class IOpenGLWrapper;

    class Shader : public IShader
    {
    public:
        enum class Type
        {
            Vertex,
            Fragment
        };

        Shader(IOpenGLWrapper* gl, Type type, std::istream& sourceStream);
        Shader(IOpenGLWrapper* gl, Type type, const std::string& source);

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) = default;
        Shader& operator=(Shader&&) = default;

        GLuint Handle() override;

    private:
        IOpenGLWrapper* _gl;

        typedef UniqueHandle<std::function<void (GLuint)>> UniqueShaderHandle;
        UniqueShaderHandle _handle;
    };
}
