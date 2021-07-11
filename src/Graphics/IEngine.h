#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IImage.h"
#include "ITileAtlas.h"

namespace Graphics
{
    class IEngine
    {
    public:
        virtual std::unique_ptr<ITileAtlas> CreateTileAtlas(
            IImage &tileAtlas,
            const glm::vec2& atlasSizeInTiles) = 0;

    protected:
        // This interface is not intended to expose ownership of the
        // concrete object implementing it.  Therefore, do not allow
        // deletion via interface pointers.
        ~IEngine() = default;
    };
}
