#include "TileAtlas.h"

#include <memory>

#include "TileMap.h"

using namespace Graphics::OpenGL;

std::unique_ptr<Graphics::ITileMap> TileAtlas::CreateTileMap(
    const IImage& tileMapImage)
{
    return std::make_unique<TileMap>(
        &_tileMapRegistry,
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

void TileAtlas::DrawAll()
{
    for(auto& tileMap : _tileMapRegistry)
    {
        tileMap->DrawAll();
    }
}
