#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace JkEng::Graphics
{
    class ICamera2d
    {
    public:
        virtual ~ICamera2d() = default;

        virtual void Center(const glm::vec2& centerPosition) = 0;
        virtual glm::vec2 Center() = 0;

        struct Fov
        {
            Fov()
              : left(-1.0f),
                right(1.0f),
                bottom(-1.0f),
                top(1.0f)
            {

            }

            Fov(float left_, float right_, float bottom_, float top_)
              : left(left_),
                right(right_),
                bottom(bottom_),
                top(top_)
            {

            }

            float left;
            float right;
            float bottom;
            float top;
        };

        virtual void FieldOfView(Fov fov) = 0;
        virtual const Fov& FieldOfView() = 0;
    };
}
