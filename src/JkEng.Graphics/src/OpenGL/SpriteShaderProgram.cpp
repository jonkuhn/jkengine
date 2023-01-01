#include "OpenGL/SpriteShaderProgram.h"
#include "OpenGL/Shader.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/TileAtlas.h"

using namespace JkEng::Graphics::OpenGL;

namespace
{
    const char *vertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 vertex;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        // The size of the tile atlas in both the x an y dimensions in units of tiles.
        uniform vec2 tileAtlasSizeInTiles;

        // Thickness of the border of each tile in the tile atlas in both the x and y dimension
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

        // Location within the atlas of the tile to draw
        uniform vec2 atlasLocation;

        out vec2 textureCoordinate;

        void main()
        {
            vec2 sizeOfDisplayPortionOfAtlasTile = vec2(1.0f, 1.0f) - 2.0f * tileAtlasEachTileBorderThicknessInTiles;
            vec2 locationWithinTileInTiles = vec2(0.0f, 0.0f);

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
            locationWithinTileInTiles.x = tileAtlasEachTileBorderThicknessInTiles.x + (vertex.x * sizeOfDisplayPortionOfAtlasTile.x);

            // Invert Y so that top left of the tile in the tile atlas image is
            // in the top left of the displayed sprite.  This is because the
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
            //
            // Note that the y axis of the border thickness does not need inverted
            // because it is intended to represent the tile's x and y offset from
            // the upper left of the tile atlas image.
            locationWithinTileInTiles.y = tileAtlasEachTileBorderThicknessInTiles.y + (1.0 - vertex.y) * sizeOfDisplayPortionOfAtlasTile.y;

            // Note that the y axis of the atlas location does not need inverted
            // because it is intended to represent the tile's x and y offset from
            // the upper left of the tile atlas image.
            textureCoordinate = (atlasLocation + locationWithinTileInTiles) / tileAtlasSizeInTiles;
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

    ShaderProgram createSpriteShaderProgram(IOpenGLWrapper& gl)
    {
        Shader vertexShader(gl, Shader::Type::Vertex, vertexShaderSource);
        Shader fragmentShader(gl, Shader::Type::Fragment, fragmentShaderSource);

        // Note that the vertex and fragment shader instances can be destroyed
        // after the shader program has been constructed.

        ShaderProgram shaderProgram(gl, vertexShader, fragmentShader);
        return shaderProgram;
    }
}

SpriteShaderProgram::SpriteShaderProgram(IOpenGLWrapper& gl)
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
    _shaderProgram.SetUniform("tileAtlasEachTileBorderThicknessInTiles", atlas.EachTileBorderThicknessInTiles());
}

void SpriteShaderProgram::AtlasLocation(const glm::vec2& atlasLocation)
{
    _shaderProgram.SetUniform("atlasLocation", atlasLocation);
}

