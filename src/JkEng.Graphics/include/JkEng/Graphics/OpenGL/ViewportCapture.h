#pragma once
#include <string>
#include <vector>

#include "../IScreenshot.h"
#include "../Color.h"

#include "IOpenGLWrapper.h"

namespace JkEng::Graphics::OpenGL
{
    class ViewportCapture : public IScreenshot
    {
    public:
        ViewportCapture(JkEng::Graphics::OpenGL::IOpenGLWrapper& gl);
        unsigned int Width() const override;
        unsigned int Height() const override;
        Color GetPixel(unsigned int x, unsigned int y) const override;
        void SaveToFileAsRaw(const std::string& filename) const override;

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

        unsigned int GetPixelDataOffset(unsigned int x, unsigned int y) const;

        Dimensions _dimensions;
        std::vector<Color> _pixelData;

        static Dimensions GetDimensions(IOpenGLWrapper& _gl);
    };
}
