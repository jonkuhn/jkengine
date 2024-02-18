#pragma once

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../ITileMap.h"
#include "Object2d.h"
#include "Texture.h"

namespace JkEng::Graphics::OpenGL
{
    class TileAtlas;
    class TileMapDrawer;

    class TileMap final : public ITileMap
    {
    public:
        TileMap(
            Texture mapTexture,
            glm::vec2 mapSizeInTiles)
          : _object2d(),
            _mapTexture(std::move(mapTexture)),
            _mapSizeInTiles(std::move(mapSizeInTiles))
        {

        }

        TileMap(const TileMap& other) = delete;
        TileMap& operator=(const TileMap& other) = delete;

        TileMap(TileMap&& other) = default;
        TileMap& operator=(TileMap&& other) = default;

        inline void Show(bool show) override
        {
            _object2d.Show(show);
        }

        inline bool Show() const override
        {
            return _object2d.Show();
        }

        inline const Texture& MapTexture() const
        {
            return _mapTexture;
        }

        inline const glm::vec2& SizeInTiles() const
        {
            return _mapSizeInTiles;
        }

        inline void Position(const glm::vec2& position) override
        {
            _object2d.Position(position);
        }
        
        inline const glm::vec3& Position() const override
        {
            return _object2d.Position();
        }

        inline void Size(glm::vec2 size) override
        {
            _object2d.Size(size);
        }

        inline const glm::vec2& Size() const override
        {
            return _object2d.Size();
        }

        inline void Rotation(float rotationDegrees) override
        {
            _object2d.Rotation(rotationDegrees);
        }

        inline float Rotation() const override
        {
            return _object2d.Rotation();
        }

        inline void Mirror(bool mirrorX, bool mirrorY) override
        {
            return _object2d.Mirror(mirrorX, mirrorY);
        }

        inline const glm::mat4& ModelMatrix() const
        {
            return _object2d.ModelMatrix();
        }

    private:
        Object2d _object2d;
        Texture _mapTexture;
        glm::vec2 _mapSizeInTiles;
    };
}