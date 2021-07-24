#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IObjectInstance2d.h"

namespace Graphics
{
    class ITileMap
    {
    public:
        virtual ~ITileMap() = default;
        virtual std::unique_ptr<IObjectInstance2d> CreateInstance() = 0;
    };
}
