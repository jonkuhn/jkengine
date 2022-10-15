#pragma once

#include "Camera2d.h"
#include "Sprite.h"
#include "TileAtlas.h"
#include "SpriteShaderProgram.h"
#include "UnitQuadVertexArray.h"

namespace JkEng::Graphics::OpenGL
{
    class SpriteDrawer
    {
    public:
        SpriteDrawer(
            SpriteShaderProgram& spriteShaderProgram,
            UnitQuadVertexArray& unitQuadVertexArray,
            Camera2d& camera2d)

          : _spriteShaderProgram(spriteShaderProgram),
            _unitQuadVertexArray(unitQuadVertexArray),
            _camera2d(camera2d)
        {

        }

        // To not allow copy or move because they are unnecessary
        // for this class.  At least right now, there is only a
        // need for one instance anyway.  If we want them to be
        // copyable or movable in the future the members can change
        // to pointers rather than references, but constructor
        // arguments for the constructor above should stay as
        // references since passing nullptr is not valid.
        SpriteDrawer(const SpriteDrawer& other) = delete;
        SpriteDrawer& operator=(const SpriteDrawer& other) = delete;
        SpriteDrawer(SpriteDrawer&& other) = delete;
        SpriteDrawer& operator=(SpriteDrawer&& other) = delete;

        inline void SetupForDrawingFromAtlas(const TileAtlas& atlas)
        {
            _spriteShaderProgram.Use();
            _spriteShaderProgram.Atlas(atlas);
            _spriteShaderProgram.ViewMatrix(_camera2d.ViewMatrix());
            _spriteShaderProgram.ProjectionMatrix(_camera2d.ProjectionMatrix());
        }

        inline void Draw(const Sprite& sprite)
        {
            const auto& atlasLocation = sprite.AtlasLocation();
            glm::vec2 atlasLocationVec2(
                static_cast<float>(atlasLocation.x),
                static_cast<float>(atlasLocation.y)
            );
            _spriteShaderProgram.AtlasLocation(atlasLocationVec2);
            _spriteShaderProgram.ModelMatrix(sprite.ModelMatrix());
            _unitQuadVertexArray.Draw();
        }

    private:
        SpriteShaderProgram& _spriteShaderProgram;
        UnitQuadVertexArray& _unitQuadVertexArray;
        Camera2d& _camera2d;
    };
}
