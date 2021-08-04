#pragma once

#include <memory>

#include "IObjectInstance2d.h"

namespace Graphics
{
    class ITileMap
    {
    public:
        virtual ~ITileMap() = default;
        virtual std::unique_ptr<IObjectInstance2d> CreateInstance(unsigned int layer) = 0;
    };
}
