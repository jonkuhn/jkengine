#include <stdexcept>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <JkEng/Graphics/PngImage.h>
#include "MockLibPngWrapper.h"
#include "TestHelpers.h"

using namespace testing;
using namespace JkEng::Graphics;

namespace
{
    ACTION_TEMPLATE(SetMessageOnPngImageArg, HAS_1_TEMPLATE_PARAMS(unsigned, argIndex), AND_1_VALUE_PARAMS(message))
    {
        std::strncpy(std::get<argIndex>(args)->message, message, 63);
        std::get<argIndex>(args)->message[63] = '\0';
    }

    ACTION_TEMPLATE(SetWidthAndHeightOnPngImageArg, HAS_1_TEMPLATE_PARAMS(unsigned, argIndex), AND_2_VALUE_PARAMS(width, height))
    {
        std::get<argIndex>(args)->width = width;
        std::get<argIndex>(args)->height = height;
    }
}

class PngImageTests : public Test
{
public:
    PngImageTests()
        : _testFilename("test-filename.png"),
          _testErrorMessage("test-error-message")
    {

    }

protected:
    NiceMock<MockLibPngWrapper> _mockLib;
    std::string _testFilename;
    const int32_t _testImageSize = 12345;
    const int _testWidth = 777;
    const int _testHeight = 888;
    std::string _testErrorMessage;

    std::tuple<std::unique_ptr<PngImage>,
        std::unique_ptr<PngImage>> GetTwoDifferentImages()
    {
        const int bytesPerPixel = 4;
        const int widthA = 111;
        const int heightA = 222;
        const int sizeInBytesA = widthA * heightA * bytesPerPixel;
        std::vector<char> dataA(sizeInBytesA);
        std::fill(dataA.begin(), dataA.end(), 'A');

        const int widthB = 333;
        const int heightB = 444;
        const int sizeInBytesB = widthB * heightB * bytesPerPixel;
        std::vector<char> dataB(sizeInBytesB);
        std::fill(dataA.begin(), dataA.end(), 'B');

        Sequence seq;
        EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, _)).InSequence(seq).WillOnce(Return(true));
        EXPECT_CALL(_mockLib, png_image_size(_)).InSequence(seq).WillOnce(Return(sizeInBytesA));
        EXPECT_CALL(_mockLib, png_image_finish_read(_, _, _, _, _)).InSequence(seq)
            .WillOnce(DoAll(
                SetWidthAndHeightOnPngImageArg<0>(widthA, heightA),
                FillArgPointeeBuffer<2>(dataA.data(), sizeInBytesA),
                Return(true)));
        EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, _)).InSequence(seq).WillOnce(Return(true));
        EXPECT_CALL(_mockLib, png_image_size(_)).InSequence(seq).WillOnce(Return(sizeInBytesB));
        EXPECT_CALL(_mockLib, png_image_finish_read(_, _, _, _, _)).InSequence(seq)
            .WillOnce(DoAll(
                SetWidthAndHeightOnPngImageArg<0>(widthB, heightB),
                FillArgPointeeBuffer<2>(dataB.data(), sizeInBytesB),
                Return(true)));

        auto imageA = std::make_unique<PngImage>(&_mockLib, "imageA.png");
        auto imageB = std::make_unique<PngImage>(&_mockLib, "imageB.png");

        return std::tuple(std::move(imageA), std::move(imageB));
    }
};

TEST_F(PngImageTests, Constructor_MakesCallsToLoadPng)
{
    // These will be pointers to a local variable in the constructor.
    // Therefore, only check them for equality, do not dereference
    // them in the test because they will no longer be valid
    png_image* capturedImagePtr1 = nullptr;
    png_image* capturedImagePtr2 = nullptr;
    png_image* capturedImagePtr3 = nullptr;

    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, StrEq(_testFilename.c_str())))
        .WillOnce(DoAll(SaveArg<0>(&capturedImagePtr1), Return(true)));
    EXPECT_CALL(_mockLib, png_image_size(_))
        .WillOnce(DoAll(SavePointerToArgReferee<0>(&capturedImagePtr2), Return(_testImageSize)));
    EXPECT_CALL(_mockLib, png_image_finish_read(_, Eq(nullptr), Ne(nullptr), Eq(0), Eq(nullptr)))
        .WillOnce(DoAll(SaveArg<0>(&capturedImagePtr3), Return(true)));

    PngImage image(&_mockLib, _testFilename);

    EXPECT_EQ(capturedImagePtr1, capturedImagePtr2);
    EXPECT_EQ(capturedImagePtr1, capturedImagePtr3);
}

TEST_F(PngImageTests, Constructor_SetsWidthHeightPixelFormatAndSize)
{
    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, StrEq(_testFilename.c_str())))
        .WillOnce(Return(true));
    EXPECT_CALL(_mockLib, png_image_size(_))
        .WillOnce(Return(_testImageSize));
    EXPECT_CALL(_mockLib, png_image_finish_read(_, Eq(nullptr), Ne(nullptr), Eq(0), Eq(nullptr)))
        .WillOnce(DoAll(SetWidthAndHeightOnPngImageArg<0>(_testWidth, _testHeight), Return(true)));

    PngImage image(&_mockLib, _testFilename);

    EXPECT_EQ(image.Width(), _testWidth);
    EXPECT_EQ(image.Height(), _testHeight);

    // Currently expect all images to be in RGBA format once loaded
    EXPECT_EQ(image.Format(), PngImage::PixelFormat::RGBA);
}

TEST_F(PngImageTests, Constructor_BeginReadFails_ThrowsRuntimeErrorContainingMessage)
{
    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, _))
        .WillOnce(DoAll(SetMessageOnPngImageArg<0>(_testErrorMessage.c_str()), Return(false)));
    EXPECT_THROW(
        try
        {
            PngImage image(&_mockLib, _testFilename);
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("begin"));
            EXPECT_THAT(e.what(), HasSubstr(_testErrorMessage));
            throw;
        },
        std::runtime_error);
}

TEST_F(PngImageTests, Constructor_FinishReadFails_ThrowsRuntimeErrorContainingMessage)
{
    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(_mockLib, png_image_finish_read(_, _, _, _, _))
        .WillOnce(DoAll(SetMessageOnPngImageArg<0>(_testErrorMessage.c_str()), Return(false)));
    EXPECT_THROW(
        try
        {
            PngImage image(&_mockLib, _testFilename);
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("finish"));
            EXPECT_THAT(e.what(), HasSubstr(_testErrorMessage));
            throw;
        },
        std::runtime_error);
}

TEST_F(PngImageTests, CopyConstructor_CreatesIdenticalCopy)
{
    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, StrEq(_testFilename.c_str())))
        .WillOnce(Return(true));
    EXPECT_CALL(_mockLib, png_image_size(_))
        .WillOnce(Return(_testImageSize));
    EXPECT_CALL(_mockLib, png_image_finish_read(_, Eq(nullptr), Ne(nullptr), Eq(0), Eq(nullptr)))
        .WillOnce(DoAll(SetWidthAndHeightOnPngImageArg<0>(_testWidth, _testHeight), Return(true)));

    PngImage original(&_mockLib, _testFilename);
    PngImage copy(original);

    // Attributes must be equal
    EXPECT_EQ(original.Width(), copy.Width());
    EXPECT_EQ(original.Height(), copy.Height());
    EXPECT_EQ(original.Format(), copy.Format());

    // Buffer must be copied not shared
    EXPECT_NE(original.Data(), copy.Data());

    // Buffer content must be the same
    ASSERT_EQ(original.SizeInBytes(), copy.SizeInBytes());
    EXPECT_TRUE(std::memcmp(original.Data(), copy.Data(), original.SizeInBytes()) == 0);
}

TEST_F(PngImageTests, CopyAssignment_CreatesIdenticalCopy)
{
    std::unique_ptr<PngImage> original;
    std::unique_ptr<PngImage> copy;
    std::tie(original, copy) = GetTwoDifferentImages();

    *original = *copy;

    // Attributes must be equal
    EXPECT_EQ(original->Width(), copy->Width());
    EXPECT_EQ(original->Height(), copy->Height());
    EXPECT_EQ(original->Format(), copy->Format());

    // Buffer must be copied not shared
    EXPECT_NE(original->Data(), copy->Data());

    // Buffer content must be the same
    ASSERT_EQ(original->SizeInBytes(), copy->SizeInBytes());
    EXPECT_TRUE(std::memcmp(original->Data(), copy->Data(), original->SizeInBytes()) == 0);
}

TEST_F(PngImageTests, MoveConstructor_TargetTakesOwnershipOfBufferAndPropertiesMatch)
{
    EXPECT_CALL(_mockLib, png_image_begin_read_from_file(_, StrEq(_testFilename.c_str())))
        .WillOnce(Return(true));
    EXPECT_CALL(_mockLib, png_image_size(_))
        .WillOnce(Return(_testImageSize));
    EXPECT_CALL(_mockLib, png_image_finish_read(_, Eq(nullptr), Ne(nullptr), Eq(0), Eq(nullptr)))
        .WillOnce(DoAll(SetWidthAndHeightOnPngImageArg<0>(_testWidth, _testHeight), Return(true)));

    PngImage source(&_mockLib, _testFilename);
    auto sourceOriginalWidth = source.Width();
    auto sourceOriginalHeight = source.Height();
    auto sourceOriginalFormat = source.Format();
    auto sourceOriginalData = source.Data();
    auto sourceOriginalSizeInBytes = source.SizeInBytes();

    PngImage target(std::move(source));

    // Attributes match those of the original source
    EXPECT_EQ(target.Width(), sourceOriginalWidth);
    EXPECT_EQ(target.Height(), sourceOriginalHeight);
    EXPECT_EQ(target.Format(), sourceOriginalFormat);
    EXPECT_EQ(target.SizeInBytes(), sourceOriginalSizeInBytes);

    // Target should directly take ownerhsip of exact source buffer
    EXPECT_EQ(target.Data(), sourceOriginalData);
}

TEST_F(PngImageTests, MoveAssignment_TargetTakesOwnershipOfBufferAndPropertiesMatch)
{
    std::unique_ptr<PngImage> source;
    std::unique_ptr<PngImage> target;
    std::tie(source, target) = GetTwoDifferentImages();

    auto sourceOriginalWidth = source->Width();
    auto sourceOriginalHeight = source->Height();
    auto sourceOriginalFormat = source->Format();
    auto sourceOriginalData = source->Data();
    auto sourceOriginalSizeInBytes = source->SizeInBytes();

    *target = std::move(*source);

    // Attributes match those of the original source
    EXPECT_EQ(target->Width(), sourceOriginalWidth);
    EXPECT_EQ(target->Height(), sourceOriginalHeight);
    EXPECT_EQ(target->Format(), sourceOriginalFormat);
    EXPECT_EQ(target->SizeInBytes(), sourceOriginalSizeInBytes);

    // Target should directly take ownerhsip of exact source buffer
    EXPECT_EQ(target->Data(), sourceOriginalData);
}
