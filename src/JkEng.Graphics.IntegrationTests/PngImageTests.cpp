#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <JkEng/Graphics/PngImage.h>
#include <JkEng/Graphics/LibPngWrapper.h>

using namespace testing;
using namespace JkEng::Graphics;

class PngImageTests : public Test
{
public:
    PngImageTests()
        : _libpng()
    {
    }
protected:
    LibPngWrapper _libpng;
};

TEST_F(PngImageTests, LoadPngImage_TopOfImageAtBeginningOfBuffer)
{
    PngImage image(&_libpng, "TestFiles/test31x47.png");

    EXPECT_EQ(image.Width(), 31);
    EXPECT_EQ(image.Height(), 47);
    EXPECT_EQ(image.Format(), PngImage::PixelFormat::RGBA);

    // 3 black rows
    for (int i = 0; i < 3*31*3; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 0);
        EXPECT_EQ(image.Data()[i+1], 0);
        EXPECT_EQ(image.Data()[i+2], 0);
        EXPECT_EQ(image.Data()[i+3], 255);
    }

    // 3 red rows
    for (int i = 3*31*4; i < 6*31*4; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 255);
        EXPECT_EQ(image.Data()[i+1], 0);
        EXPECT_EQ(image.Data()[i+2], 0);
        EXPECT_EQ(image.Data()[i+3], 255);
    }

    // 3 green rows
    for (int i = 6*31*4; i < 9*31*4; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 0);
        EXPECT_EQ(image.Data()[i+1], 255);
        EXPECT_EQ(image.Data()[i+2], 0);
        EXPECT_EQ(image.Data()[i+3], 255);
    }

    // 3 blue rows
    for (int i = 9*31*4; i < 12*31*4; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 0);
        EXPECT_EQ(image.Data()[i+1], 0);
        EXPECT_EQ(image.Data()[i+2], 255);
        EXPECT_EQ(image.Data()[i+3], 255);
    }

    // 3 transparent rows
    for (int i = 12*31*4; i < 15*31*4; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 255);
        EXPECT_EQ(image.Data()[i+1], 255);
        EXPECT_EQ(image.Data()[i+2], 255);
        EXPECT_EQ(image.Data()[i+3], 0);
    }

    // remaining rows are white
    for (int i = 15*31*4; i < 47*31*4; i+=4)
    {
        EXPECT_EQ(image.Data()[i], 255);
        EXPECT_EQ(image.Data()[i+1], 255);
        EXPECT_EQ(image.Data()[i+2], 255);
        EXPECT_EQ(image.Data()[i+3], 255);
    }
}

TEST_F(PngImageTests, LoadNonexistantFile_ThrowsRuntimeError)
{
    EXPECT_THROW(
        try
        {
            PngImage image(&_libpng, "TestFiles/DoesNotExist.file");

        }
        catch (std::runtime_error& e)
        {
            // I am not sure if these messages from libpng are OS-specific
            // if they are this expectation may need tweaked if it makes
            // the test too brittle.
            EXPECT_THAT(e.what(), HasSubstr("No such file"));
            throw;
        },
        std::runtime_error);
}

TEST_F(PngImageTests, LoadNonPngImage_ThrowsRuntimeError)
{
    EXPECT_THROW(
        try
        {
            PngImage image(&_libpng, "TestFiles/test31x47.jpg");
        }
        catch (std::runtime_error& e)
        {
            // I am not sure if these messages from libpng are OS-specific
            // if they are this expectation may need tweaked if it makes
            // the test too brittle.
            EXPECT_THAT(e.what(), HasSubstr("Not a PNG"));
            throw;
        },
        std::runtime_error);
}
