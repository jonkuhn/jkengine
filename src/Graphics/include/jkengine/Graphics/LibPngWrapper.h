#pragma once
#include "ILibPngWrapper.h"

namespace Graphics
{
    class LibPngWrapper final : public ILibPngWrapper
    {
    public:
        LibPngWrapper() {}

        LibPngWrapper(const LibPngWrapper&) = delete;
        LibPngWrapper& operator=(const LibPngWrapper&) = delete;
    
        int png_image_begin_read_from_file(png_image* image, const char *file_name) override
        {
            return ::png_image_begin_read_from_file(image, file_name);
        }

        int png_image_finish_read(png_image* image, png_color* background, void *buffer, int32_t row_stride, void *colormap) override
        {
            return ::png_image_finish_read(image, background, buffer, row_stride, colormap);
        }

        size_t png_image_size(png_image& image) override
        {
            return PNG_IMAGE_SIZE(image);
        }

        size_t png_image_row_stride(png_image& image) override
        {
            return PNG_IMAGE_ROW_STRIDE(image);
        }
    };
}
