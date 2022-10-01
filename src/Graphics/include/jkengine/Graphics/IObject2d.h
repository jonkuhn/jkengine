#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace Graphics
{
    class IObject2d
    {
    public:
        virtual ~IObject2d() = default;

        virtual void Show(bool show) = 0;
        virtual bool Show() const = 0;
        virtual void Position(const glm::vec2& position) = 0;
        virtual const glm::vec3& Position() const = 0;
        virtual void Size(glm::vec2 size) = 0;
        virtual const glm::vec2& Size() const = 0;
        virtual void Rotation(float rotationDegrees) = 0;
        virtual float Rotation() const = 0;
    };
}
