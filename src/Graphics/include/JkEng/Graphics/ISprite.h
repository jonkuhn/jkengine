#pragma once

#include <memory>

#include "IObject2d.h"
#include "GridLocation.h"

namespace JkEng::Graphics
{
    class ISprite : public IObject2d
    {
    public:
        virtual ~ISprite() = default;
        virtual void AtlasLocation(GridLocation atlasLocation) = 0;
        virtual const GridLocation& AtlasLocation() const = 0;
    };
}
