#pragma once

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include <jkengine/Shared/Registry.h>

#include "../ITileAtlas.h"
#include "Texture.h"

namespace Graphics::OpenGL
{
    class IOpenGLWrapper;
    class TileMapDrawer;
    class TileMap;
    class SpriteDrawer;
    class Sprite;

    class TileAtlas final : public ITileAtlas
    {
    public:
        TileAtlas(
            Shared::Registry<TileAtlas>& tileAtlasRegistry,
            IOpenGLWrapper& gl,
            unsigned int numberOfDrawingLayers,
            TileMapDrawer& tileMapDrawer,
            SpriteDrawer& spriteDrawer,
            Texture atlasTexture,
            glm::vec2 atlasSizeInTiles)
          : 
            _gl(&gl),
            _tileMapDrawer(&tileMapDrawer),
            _spriteDrawer(&spriteDrawer),
            _atlasTexture(std::move(atlasTexture)),
            _atlasSizeInTiles(std::move(atlasSizeInTiles)),
            _registration(tileAtlasRegistry, *this),
            _perLayerTileMapRegistries(numberOfDrawingLayers),
            _perLayerSpriteRegistries(numberOfDrawingLayers)
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

        std::unique_ptr<Graphics::ITileMap> CreateTileMap(unsigned int layer, const IImage& tileMapImage) override;
        std::unique_ptr<Graphics::ISprite> CreateSprite(unsigned int layer) override;

        inline const Texture& AtlasTexture() const
        {
            return _atlasTexture;
        }

        inline const glm::vec2& SizeInTiles() const
        {
            return _atlasSizeInTiles;
        }

        void DrawAllOnLayer(unsigned int layer);

    private:
        IOpenGLWrapper* _gl;
        TileMapDrawer* _tileMapDrawer;
        SpriteDrawer* _spriteDrawer;
        Texture _atlasTexture;
        glm::vec2 _atlasSizeInTiles;
        Shared::Registry<TileAtlas>::Registration _registration;
        std::vector<Shared::Registry<TileMap>> _perLayerTileMapRegistries;
        std::vector<Shared::Registry<Sprite>> _perLayerSpriteRegistries;
    };
}
