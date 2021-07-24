#pragma once

#include <unordered_set>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileAtlas.h"
#include "Registry.h"
#include "Texture.h"

namespace Graphics::OpenGL
{
    class Camera2d;
    class IOpenGLWrapper;
    class TileMapShaderProgram;
    class UnitQuadVertexArray;
    class TileMap;

    class TileAtlas final : public ITileAtlas
    {
    public:
        TileAtlas(
            Registry<TileAtlas>* tileAtlasRegistry,
            IOpenGLWrapper* gl,
            TileMapShaderProgram* tileMapShaderProgram,
            UnitQuadVertexArray* unitQuadVertexArray,
            Camera2d* camera2d,
            Texture atlasTexture,
            glm::vec2 atlasSizeInTiles)
          : 
            _gl(gl),
            _tileMapShaderProgram(tileMapShaderProgram),
            _unitQuadVertexArray(unitQuadVertexArray),
            _camera2d(camera2d),
            _atlasTexture(std::move(atlasTexture)),
            _atlasSizeInTiles(std::move(atlasSizeInTiles)),
            _registration(tileAtlasRegistry, this),
            _tileMapRegistry()
        {

        }

        // To not allow copy because of Registry and Registration
        // not being copyable
        TileAtlas(const TileAtlas& other) = delete;
        TileAtlas& operator=(const TileAtlas& other) = delete;

        // To not allow move because of Registry and Registration
        // not being moveable
        TileAtlas(TileAtlas&& other) = delete;
        TileAtlas& operator=(TileAtlas&& other) = delete;

        std::unique_ptr<Graphics::ITileMap> CreateTileMap(const IImage& tileMapImage) override;

        inline Texture& AtlasTexture()
        {
            return _atlasTexture;
        }

        inline const glm::vec2& Size()
        {
            return _atlasSizeInTiles;
        }

        void DrawAll();

    private:
        IOpenGLWrapper* _gl;
        TileMapShaderProgram* _tileMapShaderProgram;
        UnitQuadVertexArray* _unitQuadVertexArray;
        Camera2d* _camera2d;
        Texture _atlasTexture;
        glm::vec2 _atlasSizeInTiles;
        Registry<TileAtlas>::Registration _registration;
        Registry<TileMap> _tileMapRegistry;
    };
}
