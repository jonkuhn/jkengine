#include "PngImage.h"

#include <sstream>

#include "ILibPngWrapper.h"

using namespace Graphics;

PngImage::PngImage(ILibPngWrapper* libPng, const std::string& filename)
    : _libPng(libPng),
      _data(),
      _width(0),
      _height(0),
      _pixelFormat(PngImage::PixelFormat::Invalid)
{
    png_image image;
    memset(&image, 0, sizeof image);
    image.version = PNG_IMAGE_VERSION;
    if (!_libPng->png_image_begin_read_from_file(&image, filename.c_str()))
    {
        std::stringstream ss;
        ss << "Unable to begin reading png: " << filename
            << " libpng message: " << image.message;
        throw std::runtime_error(ss.str().c_str());
    }

    image.format = PNG_FORMAT_RGBA;
    _data.resize(_libPng->png_image_size(image));

    // Note: passing 0 for row_stride make libpng calculate it
    if (!_libPng->png_image_finish_read(&image, nullptr, _data.data(), 0, nullptr))
    {
        std::stringstream ss;
        ss << "Unable to finish reading png: " << filename
            << " libpng message: " << image.message;
        throw std::runtime_error(ss.str().c_str());
    }

    _width = image.width;
    _height = image.height;

    // Hardcoded because we request RGBA from libpng above
    _pixelFormat = PixelFormat::RGBA;
}

const uint8_t* PngImage::Data() const
{
    return _data.data();
}

int PngImage::Width() const
{
    return _width;
}

int PngImage::Height() const
{
    return _height;
}

IImage::PixelFormat PngImage::Format() const
{
    return _pixelFormat;
}

size_t PngImage::SizeInBytes() const
{
    return _data.size();
}
