#pragma once

#include "Camera2d.h"
#include "Sprite.h"
#include "TileAtlas.h"
#include "SpriteShaderProgram.h"
#include "UnitQuadVertexArray.h"

namespace Graphics::OpenGL
{
    class SpriteDrawer
    {
    public:
        SpriteDrawer(
            SpriteShaderProgram* spriteShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Camera2d* camera2d)

          : _spriteShaderProgram(spriteShaderProgram),
            _unitQuadVertexArray(unitQuadVertexArray),
            _camera2d(camera2d)
        {

        }

        inline void SetupForDrawingFromAtlas(const TileAtlas& atlas)
        {
            _spriteShaderProgram->Use();
            _spriteShaderProgram->Atlas(atlas);
            _spriteShaderProgram->ViewMatrix(_camera2d->ViewMatrix());
            _spriteShaderProgram->ProjectionMatrix(_camera2d->ProjectionMatrix());
        }

        inline void Draw(const Sprite& sprite)
        {
            _spriteShaderProgram->AtlasLocation(sprite.AtlasLocation());
            _spriteShaderProgram->ModelMatrix(sprite.ModelMatrix());
            _unitQuadVertexArray->Draw();
        }

    private:
        SpriteShaderProgram* _spriteShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Camera2d* _camera2d;
    };
}
