#pragma once

#include <vector>

#include <jkengine/Graphics/IImage.h>

class FakeImage : public ::Graphics::IImage {
public:
    FakeImage(int width, int height, ::Graphics::IImage::PixelFormat format)
        : _width(width),
          _height(height),
          _format(format),
          _data()
    {
        _data.resize(width * height * BytesPerPixel(format));
    }

    const uint8_t* Data() const override
    {
        return _data.data();
    }

    int Width() const override
    {
        return _width;
    }

    int Height() const override
    {
        return _height;
    }

    ::Graphics::IImage::PixelFormat Format() const override
    {
        return _format;
    }

private:
    int _width;
    int _height;
    ::Graphics::IImage::PixelFormat _format;
    std::vector<uint8_t> _data;

    int BytesPerPixel(::Graphics::IImage::PixelFormat format)
    {
        switch(format)
        {
            case ::Graphics::IImage::PixelFormat::Grayscale: return 1;
            case ::Graphics::IImage::PixelFormat::GrayscaleAlpha: return 2;
            case ::Graphics::IImage::PixelFormat::RGB: return 3;
            case ::Graphics::IImage::PixelFormat::RGBA: return 4;
            default: return 4;
        }
    }
};
