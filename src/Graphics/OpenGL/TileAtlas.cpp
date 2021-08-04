#include "TileAtlas.h"

#include <memory>

#include "TileMap.h"
#include "SpriteDrawer.h"
#include "SpriteInstance.h"

using namespace Graphics::OpenGL;

std::unique_ptr<Graphics::ITileMap> TileAtlas::CreateTileMap(
    const IImage& tileMapImage)
{
    return std::make_unique<TileMap>(
        &_tileMapRegistry,
        _numberOfDrawingLayers,
        _tileMapDrawer,
        this,
        Texture(
            _gl,
            Texture::Params(tileMapImage)
                .WrapModeS(Texture::WrapMode::ClampToBorder)
                .WrapModeT(Texture::WrapMode::ClampToBorder)
                .MinFilter(Texture::MinFilterMode::Nearest)
                .MagFilter(Texture::MagFilterMode::Nearest)),
        glm::vec2(tileMapImage.Width(), tileMapImage.Height())
    );
}

std::unique_ptr<Graphics::ISpriteInstance> TileAtlas::CreateSpriteInstance(unsigned int layer)
{
    return std::make_unique<SpriteInstance>(&_perLayerSpriteInstanceRegistries[layer]);
}

void TileAtlas::DrawAllOnLayer(unsigned int layer)
{
    for(auto& tileMap : _tileMapRegistry)
    {
        tileMap->DrawAllOnLayer(layer);
    }

    _spriteDrawer->SetupForDrawingInstances(*this);
    for(auto* spriteInstance : _perLayerSpriteInstanceRegistries[layer])
    {
        _spriteDrawer->DrawInstance(*spriteInstance);
    }
}
