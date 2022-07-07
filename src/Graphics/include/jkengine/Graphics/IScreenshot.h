#pragma once

#include "Color.h"

namespace Graphics
{
    class IScreenshot
    {
    public:
        virtual ~IScreenshot() = default;
        virtual unsigned int Width() const = 0;
        virtual unsigned int Height() const = 0;
        virtual Color GetPixel(unsigned int x, unsigned int y) const = 0;
        virtual void SaveToFileAsRaw(const std::string& filename) const = 0;
    };
}
