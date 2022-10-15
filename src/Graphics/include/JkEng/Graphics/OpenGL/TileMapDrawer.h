#pragma once

#include "Camera2d.h"
#include "Object2d.h"
#include "TileAtlas.h"
#include "TileMap.h"
#include "TileMapShaderProgram.h"
#include "UnitQuadVertexArray.h"

namespace JkEng::Graphics::OpenGL
{
    class TileMapDrawer
    {
    public:
        TileMapDrawer(
            TileMapShaderProgram& tileMapShaderProgram,
            UnitQuadVertexArray& unitQuadVertexArray,
            Camera2d& camera2d)

          : _tileMapShaderProgram(tileMapShaderProgram),
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
        TileMapDrawer(const TileMapDrawer& other) = delete;
        TileMapDrawer& operator=(const TileMapDrawer& other) = delete;
        TileMapDrawer(TileMapDrawer&& other) = delete;
        TileMapDrawer& operator=(TileMapDrawer&& other) = delete;

        inline void SetupForDrawingFromAtlas(const TileAtlas& atlas)
        {
            _tileMapShaderProgram.Use();
            _tileMapShaderProgram.Atlas(atlas);
            _tileMapShaderProgram.ViewMatrix(_camera2d.ViewMatrix());
            _tileMapShaderProgram.ProjectionMatrix(_camera2d.ProjectionMatrix());
        }

        inline void Draw(const TileMap& map)
        {
            _tileMapShaderProgram.Map(map);
            _tileMapShaderProgram.ModelMatrix(map.ModelMatrix());
            _unitQuadVertexArray.Draw();
        }

    private:
        TileMapShaderProgram& _tileMapShaderProgram;
        UnitQuadVertexArray& _unitQuadVertexArray;
        Camera2d& _camera2d;
    };
}
