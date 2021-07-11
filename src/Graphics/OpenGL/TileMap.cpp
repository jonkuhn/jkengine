#include "TileMap.h"

#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
    TileMapShaderProgram* tileMapShaderProgram,
    UnitQuadVertexArray* unitQuadVertexArray,
    TileAtlas* atlas,
    Texture mapTexture,
    glm::vec2 mapSizeInTiles)
    : _tileMapShaderProgram(tileMapShaderProgram),
      _unitQuadVertexArray(unitQuadVertexArray),
      _atlas(atlas),
      _mapTexture(std::move(mapTexture)),
      _mapSizeInTiles(std::move(mapSizeInTiles))
{

}

void TileMap::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    _tileMapShaderProgram->Use();
    _tileMapShaderProgram->Map(_mapTexture, _mapSizeInTiles);
    _tileMapShaderProgram->Atlas(*_atlas);
    _tileMapShaderProgram->ModelMatrix(model);
    _tileMapShaderProgram->ViewMatrix(view);
    _tileMapShaderProgram->ProjectionMatrix(projection);
    _unitQuadVertexArray->Draw();
}
