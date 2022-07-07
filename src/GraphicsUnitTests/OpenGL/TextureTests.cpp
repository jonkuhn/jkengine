#include <memory>
#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <jkengine/Graphics/OpenGL/Texture.h>
#include "../TestHelpers.h"
#include "MockOpenGLWrapper.h"
#include "../FakeImage.h"

using namespace testing;
using namespace Graphics::OpenGL;
using PixelFormat=::Graphics::IImage::PixelFormat;

class TextureTests : public Test
{
public:
    TextureTests()
    {

    }

protected:
    NiceMock<MockOpenGLWrapper> _mockLib;
    const GLuint _testHandle = 456;
    const int _testWidth = 12;
    const int _testHeight = 34;
    const PixelFormat _testPixelFormat = PixelFormat::RGBA;

    std::unique_ptr<Texture> GetTestTexture()
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        return std::make_unique<Texture>(_mockLib, Texture::Params(fakeImage));
    }

    std::unique_ptr<Texture> GetTestTextureWithPixelFormat(PixelFormat pixelFormat)
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, pixelFormat);
        return std::make_unique<Texture>(_mockLib, Texture::Params(fakeImage));
    }

    std::unique_ptr<Texture> GetTestTextureWithWrapModeS(Texture::WrapMode wrapModeS)
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        return std::make_unique<Texture>(_mockLib,
            Texture::Params(fakeImage).WrapModeS(wrapModeS));
    }

    std::unique_ptr<Texture> GetTestTextureWithWrapModeT(Texture::WrapMode wrapModeT)
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        return std::make_unique<Texture>(_mockLib,
            Texture::Params(fakeImage).WrapModeT(wrapModeT));
    }

    std::unique_ptr<Texture> GetTestTextureWithMinFilter(Texture::MinFilterMode minFilter)
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        return std::make_unique<Texture>(_mockLib,
            Texture::Params(fakeImage).MinFilter(minFilter));
    }

    std::unique_ptr<Texture> GetTestTextureWithMagFilter(Texture::MagFilterMode magFilter)
    {
        EXPECT_CALL(_mockLib, GenTextures(_, _))
            .WillOnce(SetArgPointee<1>(_testHandle));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        return std::make_unique<Texture>(_mockLib,
            Texture::Params(fakeImage).MagFilter(magFilter));
    }

    void AssertMappingForWrapModeS(Texture::WrapMode wrapMode, GLint glWrapMode)
    {
        EXPECT_CALL(_mockLib, TexParameteri(_, _, _)).Times(AnyNumber());
        EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapMode));
        GetTestTextureWithWrapModeS(wrapMode);
        Mock::VerifyAndClear(&_mockLib);
    }

    void AssertMappingForWrapModeT(Texture::WrapMode wrapMode, GLint glWrapMode)
    {
        EXPECT_CALL(_mockLib, TexParameteri(_, _, _)).Times(AnyNumber());
        EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapMode));
        GetTestTextureWithWrapModeT(wrapMode);
        Mock::VerifyAndClear(&_mockLib);
    }

    void AssertMappingForMinFilter(Texture::MinFilterMode minFilter, GLint glMinFilter)
    {
        EXPECT_CALL(_mockLib, TexParameteri(_, _, _)).Times(AnyNumber());
        EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter));
        GetTestTextureWithMinFilter(minFilter);
        Mock::VerifyAndClear(&_mockLib);
    }

    void AssertMappingForMagFilter(Texture::MagFilterMode magFilter, GLint glMagFilter)
    {
        EXPECT_CALL(_mockLib, TexParameteri(_, _, _)).Times(AnyNumber());
        EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter));
        GetTestTextureWithMagFilter(magFilter);
        Mock::VerifyAndClear(&_mockLib);
    }

    std::tuple<std::unique_ptr<Texture>,
        std::unique_ptr<Texture>> GetTwoTexturesWithHandles(GLuint handleA, GLuint handleB)
    {
        Sequence seq;
        EXPECT_CALL(_mockLib, GenTextures(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(handleA));
        EXPECT_CALL(_mockLib, GenTextures(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(handleB));
        FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
        auto textureA = std::make_unique<Texture>(_mockLib, Texture::Params(fakeImage));
        auto textureB  = std::make_unique<Texture>(_mockLib, Texture::Params(fakeImage));

        Mock::VerifyAndClear(&_mockLib);

        return std::tuple(std::move(textureA), std::move(textureB));
    }
};

TEST_F(TextureTests, Constructor_MakesCallsToCreateAndConfigureTexture)
{
    // Sequence variables to require that call order is:
    //  - GetError (to clear errors)
    //  - GenTextures
    // .- ActiveTexture (just to force it to a known state)
    //  - BindTexture(_handle)
    //  - TexParameteri and TexImage2D in any order
    //  - Generate Mipmap after TexImage2D
    //  - BindTexture(0) (to attempt to avoid future calls from affecting this texture)
    Sequence s1, s2, s3, s4, s5;

    FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);

    EXPECT_CALL(_mockLib, GetError()).InSequence(s1).WillOnce(Return(0));
    EXPECT_CALL(_mockLib, GenTextures(Eq(1), _))
        .InSequence(s1)
        .WillOnce(SetArgPointee<1>(_testHandle));
    EXPECT_CALL(_mockLib, ActiveTexture(GL_TEXTURE0)).InSequence(s1, s2, s3, s4, s5);
    EXPECT_CALL(_mockLib, BindTexture(GL_TEXTURE_2D, Eq(_testHandle))).InSequence(s1, s2, s3, s4, s5);
    EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)).InSequence(s1);
    EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)).InSequence(s2);
    EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST)).InSequence(s3);
    EXPECT_CALL(_mockLib, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)).InSequence(s4);
    EXPECT_CALL(_mockLib, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fakeImage.Width(), fakeImage.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, fakeImage.Data())).InSequence(s4);
    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).InSequence(s4);
    EXPECT_CALL(_mockLib, BindTexture(GL_TEXTURE_2D, Eq(GLuint(0)))).InSequence(s1, s2, s3, s4, s5);

    Texture texture(
        _mockLib,
        Texture::Params(fakeImage)
        .WrapModeS(Texture::WrapMode::Repeat)
        .WrapModeT(Texture::WrapMode::ClampToBorder)
        .MinFilter(Texture::MinFilterMode::NearestMipmapNearest)
        .MagFilter(Texture::MagFilterMode::Linear)
    );
}

TEST_F(TextureTests, Constructor_NonRGBAImage_Throws)
{
    EXPECT_THROW(GetTestTextureWithPixelFormat(PixelFormat::Invalid), std::logic_error);
    EXPECT_THROW(GetTestTextureWithPixelFormat(PixelFormat::Grayscale), std::logic_error);
    EXPECT_THROW(GetTestTextureWithPixelFormat(PixelFormat::GrayscaleAlpha), std::logic_error);
    EXPECT_THROW(GetTestTextureWithPixelFormat(PixelFormat::RGB), std::logic_error);
}

TEST_F(TextureTests, Constructor_InvalidWrapModeS_Throws)
{
    EXPECT_THROW(GetTestTextureWithWrapModeS(static_cast<Texture::WrapMode>(-1)), std::logic_error);
}

TEST_F(TextureTests, Constructor_InvalidWrapModeT_Throws)
{
    EXPECT_THROW(GetTestTextureWithWrapModeT(static_cast<Texture::WrapMode>(-1)), std::logic_error);
}

TEST_F(TextureTests, Constructor_InvalidMinFilter_Throws)
{
    EXPECT_THROW(GetTestTextureWithMinFilter(static_cast<Texture::MinFilterMode>(-1)), std::logic_error);
}

TEST_F(TextureTests, Constructor_InvalidMagFilter_Throws)
{
    EXPECT_THROW(GetTestTextureWithMagFilter(static_cast<Texture::MagFilterMode>(-1)), std::logic_error);
}

TEST_F(TextureTests, Constructor_WrapModeS_VerifyEnumMapping)
{
    AssertMappingForWrapModeS(Texture::WrapMode::ClampToBorder, GL_CLAMP_TO_BORDER);
    AssertMappingForWrapModeS(Texture::WrapMode::ClampToEdge, GL_CLAMP_TO_EDGE);
    AssertMappingForWrapModeS(Texture::WrapMode::MirroredRepeat, GL_MIRRORED_REPEAT);
    AssertMappingForWrapModeS(Texture::WrapMode::Repeat, GL_REPEAT);
}

TEST_F(TextureTests, Constructor_WrapModeT_VerifyEnumMapping)
{
    AssertMappingForWrapModeT(Texture::WrapMode::ClampToBorder, GL_CLAMP_TO_BORDER);
    AssertMappingForWrapModeT(Texture::WrapMode::ClampToEdge, GL_CLAMP_TO_EDGE);
    AssertMappingForWrapModeT(Texture::WrapMode::MirroredRepeat, GL_MIRRORED_REPEAT);
    AssertMappingForWrapModeT(Texture::WrapMode::Repeat, GL_REPEAT);
}

TEST_F(TextureTests, Constructor_MinFilter_VerifyEnumMapping)
{
    AssertMappingForMinFilter(Texture::MinFilterMode::Nearest, GL_NEAREST);
    AssertMappingForMinFilter(Texture::MinFilterMode::Linear, GL_LINEAR);
    AssertMappingForMinFilter(Texture::MinFilterMode::NearestMipmapNearest, GL_NEAREST_MIPMAP_NEAREST);
    AssertMappingForMinFilter(Texture::MinFilterMode::LinearMipmapNearest, GL_LINEAR_MIPMAP_NEAREST);
    AssertMappingForMinFilter(Texture::MinFilterMode::NearestMipmapLinear, GL_NEAREST_MIPMAP_LINEAR);
    AssertMappingForMinFilter(Texture::MinFilterMode::LinearMipmapLinear, GL_LINEAR_MIPMAP_LINEAR);
}

TEST_F(TextureTests, Constructor_MagFilter_VerifyEnumMapping)
{
    AssertMappingForMagFilter(Texture::MagFilterMode::Nearest, GL_NEAREST);
    AssertMappingForMagFilter(Texture::MagFilterMode::Linear, GL_LINEAR);
}

TEST_F(TextureTests, Constructor_MinFilterNoMipmap_DoesNotCallGenerateMipmap)
{
    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).Times(0);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::Nearest);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::Linear);
}

TEST_F(TextureTests, Constructor_TexImage2DThrows_ThrowsAndDeletesTexture)
{
    EXPECT_CALL(_mockLib, GenTextures(_, _))
        .WillOnce(SetArgPointee<1>(_testHandle));
    EXPECT_CALL(_mockLib, TexImage2D(_, _, _, _, _, _, _, _, _))
        .WillOnce(Throw(std::logic_error("test exception")));
    EXPECT_CALL(_mockLib, DeleteTextures(1, Pointee(Eq(_testHandle))));

    FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);
    EXPECT_THROW(
        {
            Texture texture(_mockLib, Texture::Params(fakeImage));
        },
        std::logic_error);
}

TEST_F(TextureTests, Constructor_MinFilterWithMipmap_DoesCallGenerateMipmap)

{
    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).Times(1);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::NearestMipmapNearest);
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).Times(1);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::LinearMipmapNearest);
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).Times(1);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::LinearMipmapLinear);
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, GenerateMipmap(GL_TEXTURE_2D)).Times(1);
    GetTestTextureWithMinFilter(Texture::MinFilterMode::NearestMipmapLinear);
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(TextureTests, Destructor_MakesCallToDeleteTextures)
{
    auto texture = GetTestTexture();

    EXPECT_CALL(_mockLib, DeleteTextures(1, Pointee(Eq(_testHandle))));
    texture.reset();

}

TEST_F(TextureTests, MoveConstruct_TargetCallsDeleteTexture)
{
    EXPECT_CALL(_mockLib, GenTextures(_, _)).WillOnce(SetArgPointee<1>(_testHandle));
    FakeImage fakeImage(_testWidth, _testHeight, _testPixelFormat);

    std::unique_ptr<Texture> target;
    {
        Texture source(_mockLib, Texture::Params(fakeImage));
        target = std::make_unique<Texture>(std::move(source));
    }

    // Clear mock so we can specifically assert that the target destructor
    // calls DeleteTexture
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, DeleteTextures(1, Pointee(Eq(_testHandle)))).Times(1);
    target.reset();
}

TEST_F(TextureTests, MoveAssign_MoveDeletesTargetsOriginalTexture)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Texture> source, target;
    std::tie(source, target) = GetTwoTexturesWithHandles(originalSourceHandle, originalTargetHandle);

    // Assert that target's original texture is deleted when the move occurs
    EXPECT_CALL(_mockLib, DeleteTextures(1, Pointee(Eq(originalTargetHandle)))).Times(1);
    *target = std::move(*source);

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(TextureTests, MoveAssign_SourceDestructorDeletesNothing)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Texture> source, target;
    std::tie(source, target) = GetTwoTexturesWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that deleting the source does nothing
    EXPECT_CALL(_mockLib, DeleteTextures(_, _)).Times(0);
    source.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(TextureTests, MoveAssign_TargetDestructorDeletesSourceTexture)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Texture> source, target;
    std::tie(source, target) = GetTwoTexturesWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that the source's texture (now in target) is deleted
    // when target is destroyed
    EXPECT_CALL(_mockLib, DeleteTextures(1, Pointee(Eq(originalSourceHandle)))).Times(1);
    target.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(TextureTests, Bind_MakesCallsToActivateAndBindTexture)
{
    auto texture = GetTestTexture();
    EXPECT_CALL(_mockLib, ActiveTexture(GL_TEXTURE10));
    EXPECT_CALL(_mockLib, BindTexture(GL_TEXTURE_2D, Eq(_testHandle)));
    texture->Bind(10);
}

TEST_F(TextureTests, Bind_GivenArgumentBelowMinimum_ThrowsInvalidArgument)
{
    auto texture = GetTestTexture();
    EXPECT_THROW(
        {
            texture->Bind(-1);
        },
        std::invalid_argument);
}

TEST_F(TextureTests, Bind_GivenArgumentAboveMaximum_ThrowsInvalidArgument)
{
    auto texture = GetTestTexture();
    EXPECT_THROW(
        {
            texture->Bind(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        },
        std::invalid_argument);
}
