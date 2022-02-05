#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../IObject2d.h"

#include <Shared/Registry.h>

namespace Graphics::OpenGL
{
    class Object2d : public IObject2d

    {
    public:
        Object2d()
        : _position(0.0f, 0.0f, 0.0f),
          _size(1.0f, 1.0f),
          _rotationDegrees(0.0f),
          _model(),
          _modelNeedsUpdated(true)
        {

        }

        ~Object2d()
        {

        }

        // To not allow copy because of Registry and Registration
        // not being copyable
        Object2d(const Object2d&) = delete;
        Object2d& operator=(const Object2d&) = delete;

        // To not allow move because of Registry and Registration
        // not being moveable
        Object2d(Object2d&&) = delete;
        Object2d& operator=(Object2d&&) = delete;

        inline void Position(const glm::vec2& position)
        {
            _modelNeedsUpdated = true;
            _position.x = position.x;
            _position.y = position.y;
        }
        
        inline const glm::vec3& Position() const
        {
            return _position;
        }

        inline void Size(glm::vec2 size)
        {
            _modelNeedsUpdated = true;
            _size = std::move(size);
        }

        inline const glm::vec2& Size() const
        {
            return _size;
        }

        inline void Rotation(float rotationDegrees)
        {
            _modelNeedsUpdated = true;
            _rotationDegrees = rotationDegrees;
        }

        inline float Rotation() const
        {
            return _rotationDegrees;
        }

        inline const glm::mat4& ModelMatrix() const
        {
            EnsureModelMatrixIsUpdated();
            return _model;
        }

    private:
        glm::vec3 _position;
        glm::vec2 _size;
        float _rotationDegrees;
        mutable glm::mat4 _model;  
        mutable bool _modelNeedsUpdated;

        void EnsureModelMatrixIsUpdated() const;
    };
}
