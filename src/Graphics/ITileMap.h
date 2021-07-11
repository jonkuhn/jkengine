#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace Graphics
{
    class ITileMap
    {
    public:
        virtual ~ITileMap() = default;
        virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) = 0;
    };
}
