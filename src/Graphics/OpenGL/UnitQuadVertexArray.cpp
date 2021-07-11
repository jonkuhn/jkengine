#include "UnitQuadVertexArray.h"
#include "VertexArray.h"

using namespace Graphics::OpenGL;

namespace
{
    VertexArray<Simple3dVertex> createUnitQuadVertexArray(IOpenGLWrapper *gl)
    {
        return VertexArray<Simple3dVertex>(
            gl,
            typename VertexArray<Simple3dVertex>::Params(
                std::vector<Simple3dVertex>({
                    { 1.0f, 1.0f, 0.0f },  // top right
                    { 1.0f, 0.0f, 0.0f },  // bottom right
                    { 0.0f,  0.0f, 0.0f },  // bottom left
                    { 0.0f, 1.0f, 0.0f }   // top left 
                }))
                .AddAttribute(3)
                .TriangleElementIndices(
                    std::vector<GLuint>({
                        0, 1, 3,  // first Triangle
                        1, 2, 3   // second Triangle
                    }))
        );
    }

}

UnitQuadVertexArray::UnitQuadVertexArray(IOpenGLWrapper* gl)
    : _vertexArray(createUnitQuadVertexArray(gl))
{

}

void UnitQuadVertexArray::Draw()
{
    _vertexArray.Draw();
}
