#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "../IObject2d.h"

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

        Object2d(const Object2d&) = delete;
        Object2d& operator=(const Object2d&) = delete;

        Object2d(Object2d&&) = default;
        Object2d& operator=(Object2d&&) = default;

        virtual void Show(bool show) override
        {
            _show = show;
        }

        virtual bool Show() const override
        {
            return _show;
        }

        virtual void Position(const glm::vec2& position) override
        {
            _modelNeedsUpdated = true;
            _position.x = position.x;
            _position.y = position.y;
        }
        
        virtual const glm::vec3& Position() const override
        {
            return _position;
        }

        virtual void Size(glm::vec2 size) override
        {
            _modelNeedsUpdated = true;
            _size = std::move(size);
        }

        virtual const glm::vec2& Size() const override
        {
            return _size;
        }

        virtual void Rotation(float rotationDegrees) override
        {
            _modelNeedsUpdated = true;
            _rotationDegrees = rotationDegrees;
        }

        virtual float Rotation() const override
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
        bool _show : 1 = true;

        void EnsureModelMatrixIsUpdated() const;
    };
}
