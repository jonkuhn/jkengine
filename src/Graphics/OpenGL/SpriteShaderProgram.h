#pragma once
#include <memory>

#include "IOpenGLWrapper.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace Graphics::OpenGL
{
    class TileAtlas;

    class SpriteShaderProgram final
    {
    public:
        SpriteShaderProgram(IOpenGLWrapper& gl);

        SpriteShaderProgram(SpriteShaderProgram&& other) = default;
        SpriteShaderProgram& operator=(SpriteShaderProgram&& other) = default;

        SpriteShaderProgram(const SpriteShaderProgram& other) = delete;
        SpriteShaderProgram& operator=(const SpriteShaderProgram& other) = delete;

        void Use();
        void ModelMatrix(const glm::mat4& model);
        void ViewMatrix(const glm::mat4& view);
        void ProjectionMatrix(const glm::mat4& projection);
        void Atlas(const TileAtlas& atlas);
        void AtlasLocation(const glm::vec2& atlasLocation);

    private:
        // Take a hard dependency on ShaderProgram here because this class's
        // behavior is so simple that unit testing not very beneficial.
        // Also this class is only really a SpriteShaderProgram if a very
        // specific shader program is loaded into it, so forcing that to
        // be built elsewhere and dependency injected in the name of unit
        // testing this very simple class seems like the wrong trade-off.
        ShaderProgram _shaderProgram;
    };
}