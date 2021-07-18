#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileMap.h"

#include "Texture.h"

namespace Graphics::OpenGL
{
    class Camera2d;
    class TileAtlas;
    class TileMapShaderProgram;
    class UnitQuadVertexArray;

    class TileMap final : public ITileMap
    {
    public:
        TileMap(
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Camera2d* camera2d,
            TileAtlas* atlas,
            Texture mapTexture,
            glm::vec2 mapSizeInTiles);

        TileMap(TileMap&& other) = default;
        TileMap& operator=(TileMap&& other) = default;

        TileMap(const TileMap& other) = delete;
        TileMap& operator=(const TileMap& other) = delete;
        
        void Draw(const glm::mat4& model) override;

    private:
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Camera2d* _camera2d;
        TileAtlas* _atlas;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
    };
}