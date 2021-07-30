#include "TileMap.h"

#include "Camera2d.h"
#include "TileMapDrawer.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
    Registry<TileMap>* tileMapRegistry,
    unsigned int numberOfDrawingLayers,
    TileMapDrawer* tileMapDrawer,
    TileAtlas* atlas,
    Texture mapTexture,
    glm::vec2 mapSizeInTiles)
    : _tileMapDrawer(tileMapDrawer),
      _atlas(atlas),
      _mapTexture(std::move(mapTexture)),
      _mapSizeInTiles(std::move(mapSizeInTiles)),
      _registration(tileMapRegistry, this),
      _perLayerInstanceRegistries(numberOfDrawingLayers)
{

}

std::unique_ptr<Graphics::IObjectInstance2d> TileMap::CreateInstance(unsigned int layer)
{
    return std::make_unique<ObjectInstance2d>(&_perLayerInstanceRegistries[layer]);
}

void TileMap::DrawAllOnLayer(unsigned int layer)
{
    _tileMapDrawer->SetupForDrawingInstances(*_atlas, *this);

    for(auto* instance : _perLayerInstanceRegistries[layer])
    {
        _tileMapDrawer->DrawInstance(*instance);
    }
}
