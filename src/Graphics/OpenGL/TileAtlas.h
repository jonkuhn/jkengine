#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileAtlas.h"
#include "Texture.h"
#include "TileMap.h"

namespace Graphics::OpenGL
{
    class IOpenGLWrapper;
    class TileMapShaderProgram;
    class UnitQuadVertexArray;

    class TileAtlas final : public ITileAtlas
    {
    public:
        TileAtlas(
            IOpenGLWrapper* gl,
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Texture atlasTexture,
            glm::vec2 atlasSizeInTiles)
          : 
            _gl(gl),
            _tileMapShaderProgram(tileMapShaderProgram),
            _unitQuadVertexArray(unitQuadVertexArray),
            _atlasTexture(std::move(atlasTexture)),
            _atlasSizeInTiles(std::move(atlasSizeInTiles))
        {

        }

        TileAtlas(TileAtlas&& other) = default;
        TileAtlas& operator=(TileAtlas&& other) = default;

        TileAtlas(const TileAtlas& other) = delete;
        TileAtlas& operator=(const TileAtlas& other) = delete;

        std::unique_ptr<Graphics::ITileMap> CreateTileMap(
            const IImage& tileMapImage) override
        {
            return std::make_unique<TileMap>(
                _tileMapShaderProgram,
                _unitQuadVertexArray,
                this,
                Texture(
                    _gl,
                    Texture::Params(tileMapImage)
                        .WrapModeS(Texture::WrapMode::ClampToBorder)
                        .WrapModeT(Texture::WrapMode::ClampToBorder)
                        .MinFilter(Texture::MinFilterMode::Nearest)
                        .MagFilter(Texture::MagFilterMode::Nearest)),
                glm::vec2(tileMapImage.Width(), tileMapImage.Height()));
        }

        inline Texture& AtlasTexture()
        {
            return _atlasTexture;
        }

        inline const glm::vec2& Size()
        {
            return _atlasSizeInTiles;
        }

    private:
        IOpenGLWrapper* _gl;
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Texture _atlasTexture;
        glm::vec2 _atlasSizeInTiles;
    };
}
