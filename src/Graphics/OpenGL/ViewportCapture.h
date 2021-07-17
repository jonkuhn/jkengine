#pragma once
#include <string>
#include <vector>

#include "IOpenGLWrapper.h"

#include "../IScreenshot.h"
#include "../Pixel.h"

namespace Graphics::OpenGL
{
    class ViewportCapture : public IScreenshot
    {
    public:
        ViewportCapture(Graphics::OpenGL::IOpenGLWrapper* gl);
        unsigned int Width() override;
        unsigned int Height() override;
        Pixel GetPixel(unsigned int x, unsigned int y) override;
        void SaveToFileAsRaw(const std::string& filename) override;

    private:
        struct Dimensions
        {
            Dimensions(unsigned int width_, unsigned int height_)
            {
                width = width_;
                height = height_;
            }
            unsigned int width;
            unsigned int height;
        };

        unsigned int GetPixelDataOffset(unsigned int x, unsigned int y);

        Dimensions _dimensions;
        std::vector<Pixel> _pixelData;

        static Dimensions GetDimensions(IOpenGLWrapper* _gl);
    };
}
