#pragma once

#include <unordered_set>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileAtlas.h"
#include "Registry.h"
#include "Texture.h"

namespace Graphics::OpenGL
{
    class IOpenGLWrapper;
    class TileMapDrawer;
    class TileMap;

    class TileAtlas final : public ITileAtlas
    {
    public:
        TileAtlas(
            Registry<TileAtlas>* tileAtlasRegistry,
            IOpenGLWrapper* gl,
            TileMapDrawer* tileMapDrawer,
            Texture atlasTexture,
            glm::vec2 atlasSizeInTiles)
          : 
            _gl(gl),
            _tileMapDrawer(tileMapDrawer),
            _atlasTexture(std::move(atlasTexture)),
            _atlasSizeInTiles(std::move(atlasSizeInTiles)),
            _registration(tileAtlasRegistry, this),
            _tileMapRegistry()
        {

        }

        // To not allow copy because of Registry and Registration
        // not being copyable
        TileAtlas(const TileAtlas& other) = delete;
        TileAtlas& operator=(const TileAtlas& other) = delete;

        // To not allow move because of Registry and Registration
        // not being moveable
        TileAtlas(TileAtlas&& other) = delete;
        TileAtlas& operator=(TileAtlas&& other) = delete;

        std::unique_ptr<Graphics::ITileMap> CreateTileMap(const IImage& tileMapImage) override;

        inline const Texture& AtlasTexture() const
        {
            return _atlasTexture;
        }

        inline const glm::vec2& SizeInTiles() const
        {
            return _atlasSizeInTiles;
        }

        void DrawAll();

    private:
        IOpenGLWrapper* _gl;
        TileMapDrawer* _tileMapDrawer;
        Texture _atlasTexture;
        glm::vec2 _atlasSizeInTiles;
        Registry<TileAtlas>::Registration _registration;
        Registry<TileMap> _tileMapRegistry;
    };
}
