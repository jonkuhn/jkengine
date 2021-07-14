#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IImage.h"
#include "ITileAtlas.h"
#include "IWindow.h"

namespace Graphics
{
    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        virtual std::unique_ptr<ITileAtlas> CreateTileAtlas(
            const IImage &tileAtlas,
            const glm::vec2& atlasSizeInTiles) = 0;

        virtual IWindow& GetWindow() = 0;
    };
}
