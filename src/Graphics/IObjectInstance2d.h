#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace Graphics
{
    class IObjectInstance2d
    {
    public:
        virtual ~IObjectInstance2d() = default;

        virtual void Position(glm::vec3 position) = 0;
        virtual const glm::vec3& Position() = 0;
        virtual void Size(glm::vec2 size) = 0;
        virtual const glm::vec2& Size() = 0;
        virtual void Rotation(float rotationDegrees) = 0;
        virtual float Rotation() = 0;
    };
}
