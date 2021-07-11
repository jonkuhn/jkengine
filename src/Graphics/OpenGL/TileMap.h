#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileMap.h"

#include "Texture.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

namespace Graphics::OpenGL
{
    class TileMap final : public ITileMap
    {
    public:
        TileMap(
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Texture mapTexture,
            glm::vec2 mapSizeInTiles,
            Texture atlasTexture,
            glm::vec2 atlasSizeInTiles);

        TileMap(TileMap&& other) = default;
        TileMap& operator=(TileMap&& other) = default;

        TileMap(const TileMap& other) = delete;
        TileMap& operator=(const TileMap& other) = delete;
        
        void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

    private:
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
        Texture _atlasTexture;
        glm::vec2 _atlasSizeInTiles;
    };
}