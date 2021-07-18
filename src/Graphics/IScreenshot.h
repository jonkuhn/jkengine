#pragma once

#include "Color.h"

namespace Graphics
{
    class IScreenshot
    {
    public:
        virtual ~IScreenshot() = default;
        virtual unsigned int Width() = 0;
        virtual unsigned int Height() = 0;
        virtual Color GetPixel(unsigned int x, unsigned int y) = 0;
        virtual void SaveToFileAsRaw(const std::string& filename) = 0;
    };
}
