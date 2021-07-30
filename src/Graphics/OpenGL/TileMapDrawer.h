#pragma once

#include "Camera2d.h"
#include "ObjectInstance2d.h"
#include "TileAtlas.h"
#include "TileMap.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

namespace Graphics::OpenGL
{
    class TileMapDrawer
    {
    public:
        TileMapDrawer(
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Camera2d* camera2d)

          : _tileMapShaderProgram(tileMapShaderProgram),
            _unitQuadVertexArray(unitQuadVertexArray),
            _camera2d(camera2d)
        {

        }

        inline void SetupForDrawingInstances(const TileAtlas& atlas, const TileMap& map)
        {
            _tileMapShaderProgram->Use();
            _tileMapShaderProgram->Atlas(atlas);
            _tileMapShaderProgram->Map(map);
            _tileMapShaderProgram->ViewMatrix(_camera2d->ViewMatrix());
            _tileMapShaderProgram->ProjectionMatrix(_camera2d->ProjectionMatrix());
        }

        inline void DrawInstance(const ObjectInstance2d& instance)
        {
            _tileMapShaderProgram->ModelMatrix(instance.ModelMatrix());
            _unitQuadVertexArray->Draw();
        }

    private:
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Camera2d* _camera2d;
    };
}
