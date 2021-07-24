#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileMap.h"

#include "ObjectInstance2d.h"
#include "Registry.h"
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
            Registry<TileMap>* tileMapRegistry,
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Camera2d* camera2d,
            TileAtlas* atlas,
            Texture mapTexture,
            glm::vec2 mapSizeInTiles);


        // To not allow copy because of Registry and Registration
        // not being copyable
        TileMap(const TileMap& other) = delete;
        TileMap& operator=(const TileMap& other) = delete;

        // To not allow move because of Registry and Registration
        // not being moveable
        TileMap(TileMap&& other) = delete;
        TileMap& operator=(TileMap&& other) = delete;

        std::unique_ptr<IObjectInstance2d> CreateInstance() override;
        
        void DrawAll();

    private:
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Camera2d* _camera2d;
        TileAtlas* _atlas;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
        Registry<TileMap>::Registration _registration;
        Registry<ObjectInstance2d> _instanceRegistry;

        void Draw(const glm::mat4& model);
    };
}