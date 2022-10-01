#include "OpenGL/TileAtlas.h"

#include <memory>

#include "TileAtlasDefinition.h"
#include "OpenGL/TileMapDrawer.h"
#include "OpenGL/TileMap.h"
#include "OpenGL/SpriteDrawer.h"
#include "OpenGL/Sprite.h"

using namespace Graphics::OpenGL;

TileAtlas::TileAtlas(
    IOpenGLWrapper& gl,
    TileMapDrawer& tileMapDrawer,
    SpriteDrawer& spriteDrawer,
    const TileAtlasDefinition& definition)
    : 
    _gl(&gl),
    _tileMapDrawer(&tileMapDrawer),
    _spriteDrawer(&spriteDrawer),
    _atlasTexture(
        *_gl,
        Texture::Params(definition.Image())
            .WrapModeS(Texture::WrapMode::ClampToBorder)
            .WrapModeT(Texture::WrapMode::ClampToBorder)
            .MinFilter(Texture::MinFilterMode::Nearest)
            .MagFilter(Texture::MagFilterMode::Nearest)),
    _atlasSizeInTiles(definition.AtlasSizeInTiles()),
    _perLayerTileMaps(definition.NumberOfDrawingLayers()),
    _perLayerSprites(definition.NumberOfDrawingLayers())
{
    for (size_t layer = 0; layer < _perLayerTileMaps.size(); layer++)
    {
        auto& tileMapDefinitions = definition.TileMapDefinitionsForLayer(layer);
        _perLayerTileMaps[layer].reserve(tileMapDefinitions.size());
        for (auto& tileMapDefinition : tileMapDefinitions)
        {
            _perLayerTileMaps[layer].emplace_back(
                Texture(
                    *_gl,
                    Texture::Params(tileMapDefinition.Image())
                        .WrapModeS(Texture::WrapMode::ClampToBorder)
                        .WrapModeT(Texture::WrapMode::ClampToBorder)
                        .MinFilter(Texture::MinFilterMode::Nearest)
                        .MagFilter(Texture::MagFilterMode::Nearest)),
                glm::vec2{
                    tileMapDefinition.Image().Width(),
                    tileMapDefinition.Image().Height()
                });

            // This pointer to the vector memory will be used externally
            // but it is safe because the vector will never be resized
            // because capacity was reserved above and we will only
            // add that many items to it in this loop.
            // TODO: Consider creating a wrapper for std::vector that
            // enforces this.
            tileMapDefinition.SetAfterBuildPtr(&(_perLayerTileMaps[layer].back()));
        }
    }

    for (size_t layer = 0; layer < _perLayerSprites.size(); layer++)
    {
        auto& spriteDefinitions = definition.SpriteDefinitionsForLayer(layer);
        _perLayerSprites[layer].reserve(spriteDefinitions.size());
        for (auto& spriteDefinition : spriteDefinitions)
        {
            _perLayerSprites[layer].emplace_back();

            // This pointer to the vector memory will be used externally
            // but it is safe because the vector will never be resized
            // because capacity was reserved above and we will only
            // add that many items to it in this loop.
            // TODO: Consider creating a wrapper for std::vector that
            // enforces this.
            spriteDefinition.SetAfterBuildPtr(&(_perLayerSprites[layer].back()));
        }
    }
}

void TileAtlas::DrawAllOnLayer(unsigned int layer)
{
    _tileMapDrawer->SetupForDrawingFromAtlas(*this);

    for (auto& tileMap : _perLayerTileMaps[layer])
    {
        if (tileMap.Show())
        {
            _tileMapDrawer->Draw(tileMap);
        }
    }

    _spriteDrawer->SetupForDrawingFromAtlas(*this);

    for (auto& sprite : _perLayerSprites[layer])
    {
        if (sprite.Show())
        {
            _spriteDrawer->Draw(sprite);
        }
    }
}
