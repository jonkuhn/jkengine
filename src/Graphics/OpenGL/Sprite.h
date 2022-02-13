#pragma once

#include <Shared/Registry.h>

#include "../ISprite.h"

#include "Object2d.h"

namespace Graphics::OpenGL
{
    class Sprite final : public ISprite
    {
    public:
        Sprite(Shared::Registry<Sprite>& registry)
          : _object2d(),
            _registration(registry, *this)
        {

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
        Shared::Registry<Sprite>::Registration _registration;
    };
}
