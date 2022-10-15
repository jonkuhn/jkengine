#pragma once

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "Texture.h"
#include "TileMap.h"
#include "Sprite.h"

namespace JkEng::Graphics
{
    class TileAtlasDefinition;
}

namespace JkEng::Graphics::OpenGL
{
    class IOpenGLWrapper;
    class TileMapDrawer;
    class SpriteDrawer;

    class TileAtlas final
    {
    public:
        TileAtlas(
            IOpenGLWrapper& gl,
            TileMapDrawer& tileMapDrawer,
            SpriteDrawer& spriteDrawer,
            const TileAtlasDefinition& definition);

        TileAtlas(const TileAtlas& other) = delete;
        TileAtlas& operator=(const TileAtlas& other) = delete;

        TileAtlas(TileAtlas&& other) = default;
        TileAtlas& operator=(TileAtlas&& other) = default;

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
        std::vector<std::vector<TileMap>> _perLayerTileMaps;
        std::vector<std::vector<Sprite>> _perLayerSprites;
    };
}
