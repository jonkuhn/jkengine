#pragma once

#include <functional>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include "../IObjectInstance2d.h"

namespace Graphics::OpenGL
{
    class ObjectInstance2d : public IObjectInstance2d

    {
    public:
        ObjectInstance2d(
            std::function<void(ObjectInstance2d*)> registerFunc,
            std::function<void(ObjectInstance2d*)> deregisterFunc
        )
        : _position(0.0f, 0.0f, 0.0f),
          _size(1.0f, 1.0f),
          _rotationDegrees(0.0f),
          _model(),
          _modelNeedsUpdated(true),
          _deregisterFunc(deregisterFunc)
        {
            registerFunc(this);
        }

        ~ObjectInstance2d()
        {
            _deregisterFunc(this);
        }

        ObjectInstance2d(const ObjectInstance2d&) = delete;
        ObjectInstance2d& operator=(const ObjectInstance2d&) = delete;
        ObjectInstance2d(ObjectInstance2d&&) = default;
        ObjectInstance2d& operator=(ObjectInstance2d&&) = default;

        inline void Position(glm::vec3 position)
        {
            _modelNeedsUpdated = true;
            _position = std::move(position);
        }
        
        inline const glm::vec3& Position()
        {
            return _position;
        }

        inline void Size(glm::vec2 size)
        {
            _modelNeedsUpdated = true;
            _size = std::move(size);
        }

        inline const glm::vec2& Size()
        {
            return _size;
        }

        inline void Rotation(float rotationDegrees)
        {
            _modelNeedsUpdated = true;
            _rotationDegrees = rotationDegrees;
        }

        inline float Rotation()
        {
            return _rotationDegrees;
        }

        inline const glm::mat4& ModelMatrix()
        {
            EnsureModelMatrixIsUpdated();
            return _model;
        }

    private:
        glm::vec3 _position;
        glm::vec2 _size;
        float _rotationDegrees;
        glm::mat4 _model;  
        bool _modelNeedsUpdated;
        std::function<void(ObjectInstance2d*)> _deregisterFunc;

        inline void EnsureModelMatrixIsUpdated()
        {
            if (!_modelNeedsUpdated)
            {
                return;
            }

            // The order here is very important.  Transformations take place
            // in the opposite order in which they appear here.
            // Scale is applied first.  Next, it is translated so that it is
            // centered at the origin so that rotation occurs around the middle.
            // Then, rotation is applied and it is translated back the amount it
            // was translated earlier so that position still cooresponds to 
            // to the lower left when not rotated.
            _model = glm::mat4(1.0f);
            _model = glm::translate(_model, _position);
            _model = glm::translate(_model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));  
            _model = glm::rotate(_model, glm::radians(_rotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));  
            _model = glm::translate(_model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));  
            _model = glm::scale(_model, glm::vec3(_size.x, _size.y, 0.0f));  

            _modelNeedsUpdated = false;
        }
    };
}
