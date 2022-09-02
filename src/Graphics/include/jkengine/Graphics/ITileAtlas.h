#pragma once

#include <memory>

#include <jkengine/Shared/Pool.h>

#include "IImage.h"
#include "ISprite.h"
#include "ITileMap.h"

namespace Graphics
{
    class ITileAtlas
    {
    public:
        virtual ~ITileAtlas() = default;
        virtual Shared::PoolUniquePtr<Graphics::ITileMap>::T CreateTileMap(unsigned int layer, const IImage& tileMapImage) = 0;
        virtual Shared::PoolUniquePtr<Graphics::ISprite>::T CreateSprite(unsigned int layer) = 0;
    };
}
