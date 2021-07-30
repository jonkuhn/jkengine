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
    class TileAtlas;
    class TileMapDrawer;

    class TileMap final : public ITileMap
    {
    public:
        TileMap(
            Registry<TileMap>* tileMapRegistry,
            TileMapDrawer* tileMapDrawer,
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

        inline const Texture& MapTexture() const
        {
            return _mapTexture;
        }

        inline const glm::vec2& SizeInTiles() const
        {
            return _mapSizeInTiles;
        }
        
        void DrawAll();

    private:
        TileMapDrawer* _tileMapDrawer;
        TileAtlas* _atlas;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
        Registry<TileMap>::Registration _registration;
        Registry<ObjectInstance2d> _instanceRegistry;

        void Draw(const glm::mat4& model);
    };
}