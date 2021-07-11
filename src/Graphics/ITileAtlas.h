#pragma once

#include <memory>

#include "ITileMap.h"

namespace Graphics
{
    class ITileAtlas
    {
    public:
        virtual ~ITileAtlas() = default;

        virtual std::unique_ptr<Graphics::ITileMap> CreateTileMap(IImage& tileMap) = 0;
    };
}
