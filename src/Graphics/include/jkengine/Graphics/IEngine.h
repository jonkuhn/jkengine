#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "Color.h"
#include "ICamera2d.h"
#include "IImage.h"
#include "IScreenshot.h"
#include "ITileAtlas.h"

namespace Graphics
{
    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        virtual void ClearColor(const Color &color) = 0;

        virtual Shared::RegUniquePtr<Graphics::ITileAtlas>::T CreateTileAtlas(
            const IImage& tileAtlas,
            const glm::vec2& atlasSizeInTiles) = 0;

        virtual ICamera2d* Camera2d() = 0;

        virtual void Render() = 0;

        virtual std::unique_ptr<IScreenshot> TakeScreenshot() = 0;
    };
}
