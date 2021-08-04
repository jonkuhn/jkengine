#pragma once

#include <memory>

#include "IImage.h"
#include "ISpriteInstance.h"
#include "ITileMap.h"

namespace Graphics
{
    class ITileAtlas
    {
    public:
        virtual ~ITileAtlas() = default;
        virtual std::unique_ptr<Graphics::ITileMap> CreateTileMap(const IImage& tileMap) = 0;
        virtual std::unique_ptr<Graphics::ISpriteInstance> CreateSpriteInstance(unsigned int layer) = 0;
    };
}
