#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IObjectInstance2d.h"

namespace Graphics
{
    class ISpriteInstance : public IObjectInstance2d
    {
    public:
        virtual ~ISpriteInstance() = default;
        virtual void AtlasLocation(glm::vec2 atlasLocation) = 0;
        virtual const glm::vec2& AtlasLocation() const = 0;
    };
}
