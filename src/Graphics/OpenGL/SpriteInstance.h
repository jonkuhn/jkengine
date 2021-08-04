#pragma once

#include "../ISpriteInstance.h"

#include "ObjectInstance2d.h"

namespace Graphics::OpenGL
{
    class SpriteInstance final : public ISpriteInstance
    {
    public:
        SpriteInstance(Registry<SpriteInstance>* registry)
          : _objectInstance2d(nullptr),
            _registration(registry, this)
        {

        }

        inline void Position(const glm::vec2& position) override
        {
            _objectInstance2d.Position(position);
        }
        
        inline const glm::vec3& Position() const override
        {
            return _objectInstance2d.Position();
        }

        inline void Size(glm::vec2 size) override
        {
            _objectInstance2d.Size(size);
        }

        inline const glm::vec2& Size() const override
        {
            return _objectInstance2d.Size();
        }

        inline void Rotation(float rotationDegrees) override
        {
            _objectInstance2d.Rotation(rotationDegrees);
        }

        inline float Rotation() const override
        {
            return _objectInstance2d.Rotation();
        }

        inline const glm::mat4& ModelMatrix() const
        {
            return _objectInstance2d.ModelMatrix();
        }

        inline void AtlasLocation(glm::vec2 atlasLocation) override
        {
            _atlasLocation = std::move(atlasLocation);
        }

        inline const glm::vec2& AtlasLocation() const override
        {
            return _atlasLocation;
        }

    private:
        ObjectInstance2d _objectInstance2d;
        glm::vec2 _atlasLocation;
        Registry<SpriteInstance>::Registration _registration;
    };
}
