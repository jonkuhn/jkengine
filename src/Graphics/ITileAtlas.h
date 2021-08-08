#pragma once

#include <memory>

#include "IImage.h"
#include "ISprite.h"
#include "ITileMap.h"

namespace Graphics
{
    class ITileAtlas
    {
    public:
        virtual ~ITileAtlas() = default;
        virtual std::unique_ptr<Graphics::ITileMap> CreateTileMap(unsigned int layer, const IImage& tileMapImage) = 0;
        virtual std::unique_ptr<Graphics::ISprite> CreateSprite(unsigned int layer) = 0;
    };
}
