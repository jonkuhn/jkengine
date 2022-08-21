#pragma once

#include <memory>

#include <jkengine/Shared/Registry.h>

#include "IImage.h"
#include "ISprite.h"
#include "ITileMap.h"

namespace Graphics
{
    class ITileAtlas
    {
    public:
        virtual ~ITileAtlas() = default;
        virtual Shared::RegUniquePtr<Graphics::ITileMap>::T CreateTileMap(unsigned int layer, const IImage& tileMapImage) = 0;
        virtual Shared::RegUniquePtr<Graphics::ISprite>::T CreateSprite(unsigned int layer) = 0;
    };
}
