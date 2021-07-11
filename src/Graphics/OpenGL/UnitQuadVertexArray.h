#pragma once

#include "IOpenGLWrapper.h"
#include "Simple3dVertex.h"
#include "VertexArray.h"

namespace Graphics::OpenGL
{
    // Vertex array that is a unit quad (x=0, y=0, z=0, width=1.0, height=1.0)
    class UnitQuadVertexArray final
    {
    public:
        UnitQuadVertexArray(IOpenGLWrapper* gl);
        void Draw();

    private:
        // Take a hard dependency on Vertex here because this class's
        // behavior is so simple that unit testing not very beneficial.
        // Also this class is only really a UnitQuadVertexArray if a very
        // specific VertexArray is loaded into it, so forcing that to
        // be built elsewhere and dependency injected in the name of unit
        // testing this very simple class seems like the wrong trade-off.
        VertexArray<Simple3dVertex> _vertexArray;
    };
}
