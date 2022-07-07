#pragma once

#include <gmock/gmock.h>

#include <jkengine/Graphics/ILibPngWrapper.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockLibPngWrapper : public Graphics::ILibPngWrapper
{
public:
    MOCK_METHOD(int, png_image_begin_read_from_file, (png_image* image, const char *file_name), (override));
    MOCK_METHOD(int, png_image_finish_read, (png_image* image, png_color* background, void* buffer, int32_t row_stride, void* colormap), (override));
    MOCK_METHOD(size_t, png_image_size, (png_image& image), (override));
    MOCK_METHOD(size_t, png_image_row_stride, (png_image& image), (override));
};
#pragma clang diagnostic pop
