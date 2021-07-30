#include "TileMap.h"

#include "Camera2d.h"
#include "TileMapDrawer.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
    Registry<TileMap>* tileMapRegistry,
    TileMapDrawer* tileMapDrawer,
    TileAtlas* atlas,
    Texture mapTexture,
    glm::vec2 mapSizeInTiles)
    : _tileMapDrawer(tileMapDrawer),
      _atlas(atlas),
      _mapTexture(std::move(mapTexture)),
      _mapSizeInTiles(std::move(mapSizeInTiles)),
      _registration(tileMapRegistry, this),
      _instanceRegistry()
{

}

std::unique_ptr<Graphics::IObjectInstance2d> TileMap::CreateInstance()
{
    return std::make_unique<ObjectInstance2d>(&_instanceRegistry);
}

void TileMap::DrawAll()
{
    _tileMapDrawer->SetupForDrawingInstances(*_atlas, *this);

    for(auto* instance : _instanceRegistry)
    {
        _tileMapDrawer->DrawInstance(*instance);
    }
}
