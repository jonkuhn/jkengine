#include "TileMap.h"

#include "Camera2d.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
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
      _mapSizeInTiles(std::move(mapSizeInTiles))
{

}

void TileMap::Draw(const glm::mat4& model)
{
    _tileMapShaderProgram->Use();
    _tileMapShaderProgram->Map(_mapTexture, _mapSizeInTiles);
    _tileMapShaderProgram->Atlas(*_atlas);
    _tileMapShaderProgram->ModelMatrix(model);
    _tileMapShaderProgram->ViewMatrix(_camera2d->ViewMatrix());
    _tileMapShaderProgram->ProjectionMatrix(_camera2d->ProjectionMatrix());
    _unitQuadVertexArray->Draw();
}
