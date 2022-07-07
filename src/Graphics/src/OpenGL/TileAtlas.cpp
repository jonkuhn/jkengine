#include "OpenGL/TileAtlas.h"

#include <memory>

#include "OpenGL/TileMapDrawer.h"
#include "OpenGL/TileMap.h"
#include "OpenGL/SpriteDrawer.h"
#include "OpenGL/Sprite.h"

using namespace Graphics::OpenGL;

std::unique_ptr<Graphics::ITileMap> TileAtlas::CreateTileMap(unsigned int layer, const Graphics::IImage& tileMapImage)
{
    return std::make_unique<TileMap>(
        _perLayerTileMapRegistries[layer],
        Texture(
            *_gl,
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

    _perLayerTileMapRegistries[layer].ForEach(
        [&](TileMap& tileMap)
        {
            _tileMapDrawer->Draw(tileMap);
        }
    );

    _spriteDrawer->SetupForDrawingFromAtlas(*this);

    _perLayerSpriteRegistries[layer].ForEach(
        [&](Sprite& sprite)
        {
            _spriteDrawer->Draw(sprite);
        }
    );
}
