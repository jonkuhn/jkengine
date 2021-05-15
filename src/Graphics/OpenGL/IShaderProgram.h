#pragma once

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#pragma clang diagnostic pop

namespace Graphics::OpenGL
{
    class IShaderProgram
    {
    public:
        void Use();
        void SetUniform(const std::string &name, int value);
        void SetUniform(const std::string &name, const glm::mat4& value);
        void SetUniform(const std::string &name, const glm::vec3& value);
        void SetUniform(const std::string &name, const glm::vec2& value);

        // This interface allows for deletion so that ownership can be
        // held via this interface.  This enables unit testing of use
        // cases such as TileMapShaderProgram
        virtual ~IShaderProgram() = default;
    };
}
