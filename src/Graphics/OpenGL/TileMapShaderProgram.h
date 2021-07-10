#pragma once
#include <memory>

#include "IOpenGLWrapper.h"
#include "ITexture.h"
#include "ITileMapShaderProgram.h"
#include "ShaderProgram.h"

namespace Graphics::OpenGL
{
    class TileMapShaderProgram final : public ITileMapShaderProgram
    {
    public:
        TileMapShaderProgram(IOpenGLWrapper* gl);

        TileMapShaderProgram(TileMapShaderProgram&& other) = default;
        TileMapShaderProgram& operator=(TileMapShaderProgram&& other) = default;

        TileMapShaderProgram(const TileMapShaderProgram& other) = delete;
        TileMapShaderProgram& operator=(const TileMapShaderProgram& other) = delete;

        void Use() override;
        void ModelMatrix(const glm::mat4& model) override;
        void ViewMatrix(const glm::mat4& view) override;
        void ProjectionMatrix(const glm::mat4& projection) override;
        void Map(ITexture& mapTexture, const glm::vec2  &mapSizeInTiles) override;
        void Atlas(ITexture& mapTexture, const glm::vec2& atlasSizeInTiles) override;

    private:
        // Take a hard dependency on ShaderProgram here because this class's
        // behavior is so simple that unit testing not very beneficial.
        // Also this class is only really a TileMapShaderProgram if a very
        // specific shader program is loaded into it, so forcing that to
        // be built elsewhere and dependency injected in the name of unit
        // testing this very simple class seems like the wrong trade-off.
        ShaderProgram _shaderProgram;
    };
}