#pragma once

#include <memory>

#include "IObject2d.h"

namespace JkEng::Graphics
{
    class ITileMap : public IObject2d
    {
    public:
        virtual ~ITileMap() = default;

        // In the future, methods specific to tile maps will go here
        // For example: a method to dynamically update what
        // atlas location a tile in the map points to.
    };
}
