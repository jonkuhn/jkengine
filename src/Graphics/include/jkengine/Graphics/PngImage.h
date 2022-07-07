#pragma once

#include <string>
#include <vector>

#include "IImage.h"

namespace Graphics
{
    class ILibPngWrapper;

    class PngImage final : public IImage
    {
    public:
        PngImage(ILibPngWrapper* libPng, const std::string& filename);

        PngImage(const PngImage&) = default;
        PngImage& operator=(const PngImage&) = default;
        PngImage(PngImage&&) = default;
        PngImage& operator=(PngImage&&) = default;

        const uint8_t* Data() const;
        int Width() const;
        int Height() const;
        IImage::PixelFormat Format() const;
        size_t SizeInBytes() const;

    private:
        ILibPngWrapper* _libPng;
        std::vector<uint8_t> _data;
        int _width;
        int _height;
        PixelFormat _pixelFormat;
    };
}
