#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../IObjectInstance2d.h"

#include "Registry.h"

namespace Graphics::OpenGL
{
    class ObjectInstance2d : public IObjectInstance2d

    {
    public:
        ObjectInstance2d(Registry<ObjectInstance2d>* registry)
        : _registration(registry, this),
          _position(0.0f, 0.0f, 0.0f),
          _size(1.0f, 1.0f),
          _rotationDegrees(0.0f),
          _model(),
          _modelNeedsUpdated(true)
        {

        }

        ~ObjectInstance2d()
        {

        }

        // To not allow copy because of Registry and Registration
        // not being copyable
        ObjectInstance2d(const ObjectInstance2d&) = delete;
        ObjectInstance2d& operator=(const ObjectInstance2d&) = delete;

        // To not allow move because of Registry and Registration
        // not being moveable
        ObjectInstance2d(ObjectInstance2d&&) = delete;
        ObjectInstance2d& operator=(ObjectInstance2d&&) = delete;

        inline void Position(glm::vec3 position)
        {
            _modelNeedsUpdated = true;
            _position = std::move(position);
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
        Registry<ObjectInstance2d>::Registration _registration;
        glm::vec3 _position;
        glm::vec2 _size;
        float _rotationDegrees;

        mutable glm::mat4 _model;  
        mutable bool _modelNeedsUpdated;

        void EnsureModelMatrixIsUpdated() const;
    };
}
