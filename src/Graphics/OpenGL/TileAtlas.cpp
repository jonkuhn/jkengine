#include "TileAtlas.h"

#include <memory>

#include "TileMapDrawer.h"
#include "TileMap.h"
#include "SpriteDrawer.h"
#include "Sprite.h"

using namespace Graphics::OpenGL;

std::unique_ptr<Graphics::ITileMap> TileAtlas::CreateTileMap(unsigned int layer, const Graphics::IImage& tileMapImage)
{
    return std::make_unique<TileMap>(
        _perLayerTileMapRegistries[layer],
        Texture(
            _gl,
            Texture::Params(tileMapImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        glm::vec2(tileMapImage.Width(), tileMapImage.Height()));
}

std::unique_ptr<Graphics::ISprite> TileAtlas::CreateSprite(unsigned int layer)
{
    return std::make_unique<Sprite>(_perLayerSpriteRegistries[layer]);
}

void TileAtlas::DrawAllOnLayer(unsigned int layer)
{
    _tileMapDrawer->SetupForDrawingFromAtlas(*this);

    for(auto* tileMap : _perLayerTileMapRegistries[layer])
    {
        _tileMapDrawer->Draw(*tileMap);
    }

    _spriteDrawer->SetupForDrawingFromAtlas(*this);
    for(auto* sprite : _perLayerSpriteRegistries[layer])
    {
        _spriteDrawer->Draw(*sprite);
    }
}
