#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IObject2d.h"

namespace Graphics
{
    class ISprite : public IObject2d
    {
    public:
        virtual ~ISprite() = default;
        virtual void AtlasLocation(glm::vec2 atlasLocation) = 0;
        virtual const glm::vec2& AtlasLocation() const = 0;
    };
}
