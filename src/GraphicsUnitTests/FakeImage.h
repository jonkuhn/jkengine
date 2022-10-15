#pragma once

#include <vector>

#include <JkEng/Graphics/IImage.h>

class FakeImage : public ::JkEng::Graphics::IImage {
public:
    FakeImage(int width, int height, ::JkEng::Graphics::IImage::PixelFormat format)
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

    ::JkEng::Graphics::IImage::PixelFormat Format() const override
    {
        return _format;
    }

private:
    int _width;
    int _height;
    ::JkEng::Graphics::IImage::PixelFormat _format;
    std::vector<uint8_t> _data;

    int BytesPerPixel(::JkEng::Graphics::IImage::PixelFormat format)
    {
        switch(format)
        {
            case ::JkEng::Graphics::IImage::PixelFormat::Grayscale: return 1;
            case ::JkEng::Graphics::IImage::PixelFormat::GrayscaleAlpha: return 2;
            case ::JkEng::Graphics::IImage::PixelFormat::RGB: return 3;
            case ::JkEng::Graphics::IImage::PixelFormat::RGBA: return 4;
            default: return 4;
        }
    }
};
