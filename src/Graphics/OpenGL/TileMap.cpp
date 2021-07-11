#include "TileMap.h"

using namespace Graphics::OpenGL;

TileMap::TileMap(
    TileMapShaderProgram* tileMapShaderProgram,
    UnitQuadVertexArray* unitQuadVertexArray,
    Texture mapTexture,
    glm::vec2 mapSizeInTiles,
    Texture atlasTexture,
    glm::vec2 atlasSizeInTiles)
    : _tileMapShaderProgram(tileMapShaderProgram),
      _unitQuadVertexArray(unitQuadVertexArray),
      _mapTexture(std::move(mapTexture)),
      _mapSizeInTiles(std::move(mapSizeInTiles)),
      _atlasTexture(std::move(atlasTexture)),
      _atlasSizeInTiles(std::move(atlasSizeInTiles))
{

}

void TileMap::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    _tileMapShaderProgram->Use();
    _tileMapShaderProgram->Map(_mapTexture, _mapSizeInTiles);
    _tileMapShaderProgram->Atlas(_atlasTexture, _atlasSizeInTiles);
    _tileMapShaderProgram->ModelMatrix(model);
    _tileMapShaderProgram->ViewMatrix(view);
    _tileMapShaderProgram->ProjectionMatrix(projection);
    _unitQuadVertexArray->Draw();
}
