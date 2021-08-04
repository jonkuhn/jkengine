#include "TileMapShaderProgram.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "TileAtlas.h"
#include "TileMap.h"

using namespace Graphics::OpenGL;

namespace
{
    const char *vertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 vertex;

        out vec2 tileMapLocation;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        uniform vec2 tileMapSizeInTiles;

        void main()
        {
            // tileMapLocation must be multiplied up in the vertex shader and then
            // divided back down in the fragment shader in order for interpolation
            // and rounding to work right to pick the right indices from the tile
            // map.  It cannot be calculated from tileMapLocationInWorld because
            // that is in world coordinates and we want the world coordinate
            // position of it to not affect the content of the tile mapped object.
            tileMapLocation.x = vertex.x * tileMapSizeInTiles.x;

            // Invert Y so that top right of tile map image is top right of
            // the displayed tile map
            tileMapLocation.y = (1.0 - vertex.y) * tileMapSizeInTiles.y;

            vec4 tileMapLocationInWorld = (model * vec4(vertex.xy, 1.0, 1.0));
            gl_Position = projection * view * tileMapLocationInWorld;
        }
    )GLSL";

    const char *fragmentShaderSource = R"GLSL(
        #version 330 core
        uniform vec2 tileMapSizeInTiles;
        uniform sampler2D tileMap;
        uniform sampler2D tileAtlas;
        uniform vec2 tileAtlasSizeInTiles;

        in vec2 tileMapLocation;

        out vec4 FragColor;

        void main()
        {
            vec2 tileAtlasLocation = texture(tileMap, tileMapLocation / tileMapSizeInTiles).xy * 255;
            FragColor = texture(tileAtlas, (tileAtlasLocation + fract(tileMapLocation)) / tileAtlasSizeInTiles);
        }
    )GLSL";

    ShaderProgram createTileMapShaderProgram(IOpenGLWrapper* gl)
    {
        Shader vertexShader(gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(gl, Shader::Type::Fragment, fragmentShaderSource);

        // Note that the vertex and fragment shader instances can be destroyed
        // after the shader program has been constructed.

        ShaderProgram shaderProgram(gl, {&vertexShader, &fragmentShader});
        return shaderProgram;
    }
}

TileMapShaderProgram::TileMapShaderProgram(IOpenGLWrapper* gl)
    : _shaderProgram(createTileMapShaderProgram(gl))
{

}

void TileMapShaderProgram::Use()
{
    _shaderProgram.Use();
}

void TileMapShaderProgram::ModelMatrix(const glm::mat4& model)
{
    _shaderProgram.SetUniform("model", model);
}

void TileMapShaderProgram::ViewMatrix(const glm::mat4& view)
{
    _shaderProgram.SetUniform("view", view);
}

void TileMapShaderProgram::ProjectionMatrix(const glm::mat4& projection)
{
    _shaderProgram.SetUniform("projection", projection);
}

void TileMapShaderProgram::Map(const TileMap& map)
{
    const int mapTextureIndex = 0;
    map.MapTexture().Bind(mapTextureIndex);
    _shaderProgram.SetUniform("tileMap", mapTextureIndex);
    _shaderProgram.SetUniform("tileMapSizeInTiles", map.SizeInTiles());
}

void TileMapShaderProgram::Atlas(const TileAtlas& atlas)
{
    const int atlasTextureIndex = 1;
    atlas.AtlasTexture().Bind(atlasTextureIndex);
    _shaderProgram.SetUniform("tileAtlas", atlasTextureIndex);
    _shaderProgram.SetUniform("tileAtlasSizeInTiles", atlas.SizeInTiles());
}
