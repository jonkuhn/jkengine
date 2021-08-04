#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "ViewportCapture.h"

using namespace Graphics::OpenGL;

namespace
{
    struct GLViewportDimensions
    {
        GLint x, y, width, height;
    };

    std::ostream& operator<<(std::ostream &strm, const GLViewportDimensions &d)
    {
        return strm << "ViewportDimensions(" << +d.x << ", " << +d.y << ", " << +d.width << ", " << +d.height << ")";
    }

    void ThrowIfGLViewportDimensionsViolatesExpectations(
        const GLViewportDimensions &viewportDimensions)
    {
        bool expectationsViolated = false;
        std::stringstream ss1;

        if(viewportDimensions.x != 0)
        {
            expectationsViolated = true;
            ss1 << " - expected x == 0 " << std::endl;
        }

        if(viewportDimensions.y != 0)
        {
            expectationsViolated = true;
            ss1 << " - expected y == 0 " << std::endl;
        }

        if(viewportDimensions.width <= 0)
        {
            expectationsViolated = true;
            ss1 << " - expected width > 0 " << std::endl;
        }

        if(viewportDimensions.height <= 0)
        {
            expectationsViolated = true;
            ss1 << " - expected height > 0 " << std::endl;
        }

        if (expectationsViolated)
        {
            std::stringstream ss2;
            ss2 << "OpenGL viewport dimensions violated expectations: " << std::endl
                << viewportDimensions << std::endl
                << ss1.str();
            throw std::out_of_range(ss2.str());
        }
    }
}

ViewportCapture::Dimensions ViewportCapture::GetDimensions(IOpenGLWrapper* gl)
{
    GLViewportDimensions viewportDimensions;
    gl->GetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(&viewportDimensions));
    ThrowIfGLViewportDimensionsViolatesExpectations(viewportDimensions);

    return ViewportCapture::Dimensions(
        static_cast<unsigned int>(viewportDimensions.width),
        static_cast<unsigned int>(viewportDimensions.height));
}

ViewportCapture::ViewportCapture(IOpenGLWrapper* gl)
    : _dimensions(GetDimensions(gl)),
      _pixelData(_dimensions.width * _dimensions.height)
{
    gl->ReadBuffer(GL_FRONT);
    gl->ReadPixels(0, 0, _dimensions.width, _dimensions.height, GL_RGBA, GL_UNSIGNED_BYTE, _pixelData.data());
}

unsigned int ViewportCapture::Width() const
{
    return _dimensions.width;
}

unsigned int ViewportCapture::Height() const
{
    return _dimensions.height;
}

Graphics::Color ViewportCapture::GetPixel(unsigned int x, unsigned int y) const
{
    if (x < 0 || x >= _dimensions.width)
    {
        std::stringstream ss;
        ss << "GetPixel x argument (" << x << ") must be in range 0-" << (_dimensions.width - 1);
        throw std::out_of_range(ss.str());
    }

    if (y < 0 || y >= _dimensions.height)
    {
        std::stringstream ss;
        ss << "GetPixel y argument (" << y << ") must be in range 0-" << (_dimensions.height - 1);
        throw std::out_of_range(ss.str());
    }

    return _pixelData[GetPixelDataOffset(x, y)];
}

void ViewportCapture::SaveToFileAsRaw(const std::string& filename) const
{
    // Because of OpenGL's coordinate system this will be flipped vertically

    std::ofstream file;        
    file.open(filename, std::ios::out | std::ios::binary);

    const unsigned int x = 0;
    for(unsigned int y = 0; y < _dimensions.height; y++)
    {
        auto offset = GetPixelDataOffset(x, y);
        file.write(
            reinterpret_cast<const char *>(&_pixelData[offset]),
            _dimensions.width * sizeof(Graphics::Color));
    }
    file.close();
}

unsigned int ViewportCapture::GetPixelDataOffset(unsigned int x, unsigned int y) const
{
    // GetPixel (and this function) take coordinates in a conventional
    // image/window coordinate system with the y-axis going from top to
    // bottom.  OpenGL's coordinate system has the y-axis going from
    // bottom to top.  Therefore, we invert the y coordinate to map it
    // into OpenGL's coordinate system.
    auto yInverted = (_dimensions.height - 1 - y);
    auto offset = x + yInverted * _dimensions.width;
    return offset;
}
