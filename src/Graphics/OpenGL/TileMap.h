#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileMap.h"

#include "Texture.h"

namespace Graphics::OpenGL
{
    class TileAtlas;
    class TileMapShaderProgram;
    class UnitQuadVertexArray;

    class TileMap final : public ITileMap
    {
    public:
        TileMap(
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            TileAtlas* atlas,
            Texture mapTexture,
            glm::vec2 mapSizeInTiles);

        TileMap(TileMap&& other) = default;
        TileMap& operator=(TileMap&& other) = default;

        TileMap(const TileMap& other) = delete;
        TileMap& operator=(const TileMap& other) = delete;
        
        void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;

    private:
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        TileAtlas* _atlas;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
    };
}