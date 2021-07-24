#include "TileMap.h"

#include "Camera2d.h"
#include "ObjectInstance2d.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
    Registry<TileMap>* tileMapRegistry,
    TileMapShaderProgram* tileMapShaderProgram,
    UnitQuadVertexArray* unitQuadVertexArray,
    Camera2d* camera2d,
    TileAtlas* atlas,
    Texture mapTexture,
    glm::vec2 mapSizeInTiles)
    : _tileMapShaderProgram(tileMapShaderProgram),
      _unitQuadVertexArray(unitQuadVertexArray),
      _camera2d(camera2d),
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
    _tileMapShaderProgram->Use();
    _tileMapShaderProgram->Map(_mapTexture, _mapSizeInTiles);
    _tileMapShaderProgram->Atlas(*_atlas);
    _tileMapShaderProgram->ViewMatrix(_camera2d->ViewMatrix());
    _tileMapShaderProgram->ProjectionMatrix(_camera2d->ProjectionMatrix());

    for(auto* instance : _instanceRegistry)
    {
        _tileMapShaderProgram->ModelMatrix(instance->ModelMatrix());
        _unitQuadVertexArray->Draw();
    }
}
