#pragma once

#include "../ISprite.h"

#include "Object2d.h"

namespace Graphics::OpenGL
{
    class Sprite final : public ISprite
    {
    public:
        Sprite()
        {

        }

        Sprite(const Sprite& other) = delete;
        Sprite& operator=(const Sprite& other) = delete;

        Sprite(Sprite&& other) = default;
        Sprite& operator=(Sprite&& other) = default;

        inline void Show(bool show) override
        {
            _object2d.Show(show);
        }

        inline bool Show() const override
        {
            return _object2d.Show();
        }

        inline void AtlasLocation(GridLocation atlasLocation) override
        {
            _atlasLocation = std::move(atlasLocation);
        }

        inline const GridLocation& AtlasLocation() const override
        {
            return _atlasLocation;
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

        inline const glm::mat4& ModelMatrix() const
        {
            return _object2d.ModelMatrix();
        }

    private:
        Object2d _object2d;
        GridLocation _atlasLocation;
    };
}
