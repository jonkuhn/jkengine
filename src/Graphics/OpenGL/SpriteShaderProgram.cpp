#include "SpriteShaderProgram.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "TileAtlas.h"

using namespace Graphics::OpenGL;

namespace
{
    const char *vertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 vertex;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform vec2 tileAtlasSizeInTiles;
        uniform vec2 atlasLocation;

        out vec2 textureCoordinate;

        void main()
        {
            // Calculate the texture coordinate:
            // - Start with the vertex x and y coordinates.  These will range from
            //   0.0 to 1.0 due to this being intended for use with UnitQuadVertexArray.
            // - Add to that the atlas location which is the x and y offset (in tiles)
            //   of the tile in the tile atlas that should currently be drawn for the
            //   sprite.
            // - Then multiply the sum from above by the size of an individual tile
            //   (in texture space) to get the texture space coordinate to draw.
            //
            // The result is that:
            // - vertex.xy will be used to choose what part of a given tile to draw
            //   since these values will be interpolated for each fragment of the quad.
            // - atlasLocation chooses the tile within the tile atlas texture that will
            //   be drawn.
            // - The size of the tile atlas is used (along with the knowledge that
            //   texture space coordinates span from 0.0 to 1.0) is used to convert
            //   the coordinates into texture space.
            vec2 tileSize = 1.0 / tileAtlasSizeInTiles;
            textureCoordinate = (vertex.xy + atlasLocation) * tileSize;
            gl_Position = projection * view * model * vec4(vertex.xy, 1.0, 1.0);
        }
    )GLSL";

    const char *fragmentShaderSource = R"GLSL(
        #version 330 core
        uniform sampler2D tileAtlas;

        in vec2 textureCoordinate;

        out vec4 FragColor;

        void main()
        {
            FragColor = texture(tileAtlas, textureCoordinate);
        }
    )GLSL";

    ShaderProgram createSpriteShaderProgram(IOpenGLWrapper* gl)
    {
        Shader vertexShader(gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(gl, Shader::Type::Fragment, fragmentShaderSource);

        // Note that the vertex and fragment shader instances can be destroyed
        // after the shader program has been constructed.

        ShaderProgram shaderProgram(gl, {&vertexShader, &fragmentShader});
        return shaderProgram;
    }
}

SpriteShaderProgram::SpriteShaderProgram(IOpenGLWrapper* gl)
    : _shaderProgram(createSpriteShaderProgram(gl))
{

}

void SpriteShaderProgram::Use()
{
    _shaderProgram.Use();
}

void SpriteShaderProgram::ModelMatrix(const glm::mat4& model)
{
    _shaderProgram.SetUniform("model", model);
}

void SpriteShaderProgram::ViewMatrix(const glm::mat4& view)
{
    _shaderProgram.SetUniform("view", view);
}

void SpriteShaderProgram::ProjectionMatrix(const glm::mat4& projection)
{
    _shaderProgram.SetUniform("projection", projection);
}

void SpriteShaderProgram::Atlas(const TileAtlas& atlas)
{
    const int atlasTextureIndex = 0;
    atlas.AtlasTexture().Bind(atlasTextureIndex);
    _shaderProgram.SetUniform("tileAtlas", atlasTextureIndex);
    _shaderProgram.SetUniform("tileAtlasSizeInTiles", atlas.SizeInTiles());
}

void SpriteShaderProgram::AtlasLocation(const glm::vec2& atlasLocation)
{
    _shaderProgram.SetUniform("atlasLocation", atlasLocation);
}

