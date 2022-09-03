#include "OpenGL/TileAtlas.h"

#include <memory>

#include "OpenGL/TileMapDrawer.h"
#include "OpenGL/TileMap.h"
#include "OpenGL/SpriteDrawer.h"
#include "OpenGL/Sprite.h"

using namespace Graphics::OpenGL;

Shared::PoolUniquePtr<Graphics::ITileMap>::T TileAtlas::CreateTileMap(unsigned int layer, const Graphics::IImage& tileMapImage)
{
    return _perLayerTileMapPools[layer].MakeUnique<Graphics::ITileMap>(
        Texture(
            *_gl,
            Texture::Params(tileMapImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        glm::vec2(tileMapImage.Width(), tileMapImage.Height()));
}

Shared::PoolUniquePtr<Graphics::ISprite>::T TileAtlas::CreateSprite(unsigned int layer)
{
    return _perLayerSpritePools[layer].MakeUnique<Graphics::ISprite>();
}

void TileAtlas::DrawAllOnLayer(unsigned int layer)
{
    _tileMapDrawer->SetupForDrawingFromAtlas(*this);

    _perLayerTileMapPools[layer].ForEach(
        [&](TileMap& tileMap)
        {
            _tileMapDrawer->Draw(tileMap);
        }
    );

    _spriteDrawer->SetupForDrawingFromAtlas(*this);

    _perLayerSpritePools[layer].ForEach(
        [&](Sprite& sprite)
        {
            _spriteDrawer->Draw(sprite);
        }
    );
}
