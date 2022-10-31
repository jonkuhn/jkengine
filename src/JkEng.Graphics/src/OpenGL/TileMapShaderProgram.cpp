#include "OpenGL/TileMapShaderProgram.h"
#include "OpenGL/Shader.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/TileAtlas.h"
#include "OpenGL/TileMap.h"

using namespace JkEng::Graphics::OpenGL;

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

            // Invert Y so that top left of the tile in the tile atlas image is
            // in the top left of the displayed tile.  This is because the
            // conventional y axis in an image goes from top to bottom, but
            // the y axis of our unit quad runs from bottom to top (as does our
            // world coordinate system)  OpenGL's texture y axis also runs from
            // bottom to top, but since the engine represents images in memory
            // in the conventional way such that the first pixel in the buffer
            // corresponds to the upper left of the image at coordinates (0, 0),
            // this effectively means that OpenGL texture coordinates match actual
            // image coordinates.  Since the lower left of our unit quad is (0, 0)
            // and the upper right is (1, 1), it means that to display tiles from
            // an image right-side-up we need to make y=0 of our unit quad correspond
            // to the bottom of a tile in the image and y=1 of our unit quad correspond
            // to the top of the tile in the image.  And in the image (and texture)
            // "top" has lower y values than "bottom"
            tileMapLocation.y = (1.0 - vertex.y) * tileMapSizeInTiles.y;

            vec4 tileMapLocationInWorld = (model * vec4(vertex.xy, 1.0, 1.0));
            gl_Position = projection * view * tileMapLocationInWorld;
        }
    )GLSL";

    const char *fragmentShaderSource = R"GLSL(
        #version 330 core

        // The size of the tile *map* in both the x and y dimensions in units of tiles.
        uniform vec2 tileMapSizeInTiles;

        // The tile map texture index
        uniform sampler2D tileMap;

        // The tile atlas texture index
        uniform sampler2D tileAtlas;

        // The size of the tile *atlas* in both the x an y dimensions in units of tiles.
        uniform vec2 tileAtlasSizeInTiles;

        // Thickness of the border of each tile in the tile *atlas* in both the x and y dimension
        // expressed in numbers of tiles.  Because it is expressed in numbers of tiles it can be
        // thought of as the thickness of the border as a percentage of the size of the tile.
        // The value provided for the X component is assumed to be the thickness
        // of a border that exists on both the left and right of the tile.
        // The value provided for the Y component is assumed to be the thickness
        // of a border that exists on both the top and bottom of the tile.
        //
        // It is recommended to set this to the equivalent of 1 pixel in both the x and y dimensions.
        // The border pixels in each tile should duplicate the values of the adjacent pixels
        // that are on the edge of the actual tile itself.  This will avoid artifacts due to
        // overscanning/bleeding across the boundaries of a tile.
        uniform vec2 tileAtlasEachTileBorderThicknessInTiles;

        in vec2 tileMapLocation;

        out vec4 FragColor;

        void main()
        {
            // Note that the y axis of the atlas location does not need inverted
            // because it is intended to represent the tile's x and y offset from
            // the upper left of the tile atlas image.

            vec2 sizeOfDisplayPortionOfAtlasTile = vec2(1.0f, 1.0f) - 2.0f * tileAtlasEachTileBorderThicknessInTiles;
            vec2 locationOfCornerOfTileInTiles = texture(tileMap, floor(tileMapLocation) / tileMapSizeInTiles).xy * 255;
            vec2 locationWithinTileInTiles = tileAtlasEachTileBorderThicknessInTiles + fract(tileMapLocation) * sizeOfDisplayPortionOfAtlasTile;
            FragColor = texture(tileAtlas, (locationOfCornerOfTileInTiles + locationWithinTileInTiles) / tileAtlasSizeInTiles);
        }
    )GLSL";

    ShaderProgram createTileMapShaderProgram(IOpenGLWrapper& gl)
    {
        Shader vertexShader(gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(gl, Shader::Type::Fragment, fragmentShaderSource);

        // Note that the vertex and fragment shader instances can be destroyed
        // after the shader program has been constructed.

        ShaderProgram shaderProgram(gl, vertexShader, fragmentShader);
        return shaderProgram;
    }
}

TileMapShaderProgram::TileMapShaderProgram(IOpenGLWrapper& gl)
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
    _shaderProgram.SetUniform("tileAtlasEachTileBorderThicknessInTiles", atlas.EachTileBorderThicknessInTiles());
}
