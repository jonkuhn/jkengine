#include "OpenGL/Texture.h"

#include <sstream>

using namespace Graphics::OpenGL;

namespace
{
    GLint getGlFormatFromImage(const ::Graphics::IImage& image)
    {
        switch (image.Format())
        {
            case ::Graphics::IImage::PixelFormat::RGBA: return GL_RGBA;
            default:
                std::stringstream ss;
                ss << "Unsupported texture format " << static_cast<int>(image.Format());
                throw std::logic_error(ss.str().c_str());
        }
    }

    GLint getGlWrapMode(const Texture::WrapMode wrapMode)
    {
        switch (wrapMode)
        {
            case Texture::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case Texture::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
            case Texture::WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case Texture::WrapMode::Repeat: return GL_REPEAT;
            default:
                std::stringstream ss;
                ss << "Unsupported wrap mode " << static_cast<int>(wrapMode);
                throw std::logic_error(ss.str().c_str());
        }
    }

    GLint getGlMinFilter(const Texture::MinFilterMode minFilter)
    {
        switch (minFilter)
        {
            case Texture::MinFilterMode::Nearest: return GL_NEAREST;
            case Texture::MinFilterMode::Linear: return GL_LINEAR;
            case Texture::MinFilterMode::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
            case Texture::MinFilterMode::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
            case Texture::MinFilterMode::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
            case Texture::MinFilterMode::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
            default:
                std::stringstream ss;
                ss << "Unsupported min filter " << static_cast<int>(minFilter);
                throw std::logic_error(ss.str().c_str());
        }
    }

    bool doesMinFilterRequireMipmap(const Texture::MinFilterMode minFilter)
    {
        switch (minFilter)
        {
            case Texture::MinFilterMode::Nearest: return false;
            case Texture::MinFilterMode::Linear: return false;
            case Texture::MinFilterMode::NearestMipmapNearest: return true;
            case Texture::MinFilterMode::LinearMipmapNearest: return true;
            case Texture::MinFilterMode::NearestMipmapLinear: return true;
            case Texture::MinFilterMode::LinearMipmapLinear: return true;
            default:
                std::stringstream ss;
                ss << "Unsupported min filter " << static_cast<int>(minFilter);
                throw std::logic_error(ss.str().c_str());
        }
    }

    GLint getGlMagFilter(const Texture::MagFilterMode magFilter)
    {
        switch (magFilter)
        {
            case Texture::MagFilterMode::Nearest: return GL_NEAREST;
            case Texture::MagFilterMode::Linear: return GL_LINEAR;
            default:
                std::stringstream ss;
                ss << "Unsupported min filter " << static_cast<int>(magFilter);
                throw std::logic_error(ss.str().c_str());
        }
    }
}

Texture::Params::Params(const IImage& image_)
    : image(image_),
      wrapModeS(Texture::WrapMode::Repeat),
      wrapModeT(Texture::WrapMode::Repeat),
      minFilter(Texture::MinFilterMode::NearestMipmapLinear),
      magFilter(Texture::MagFilterMode::Linear)
{

}

Texture::Params& Texture::Params::WrapModeS(WrapMode wrapModeS_)
{
    wrapModeS = wrapModeS_;
    return *this;
}

Texture::Params& Texture::Params::WrapModeT(WrapMode wrapModeT_)
{
    wrapModeT = wrapModeT_;
    return *this;
}

Texture::Params& Texture::Params::MinFilter(MinFilterMode minFilter_)
{
    minFilter = minFilter_;
    return *this;
}

Texture::Params& Texture::Params::MagFilter(MagFilterMode magFilter_)
{
    magFilter = magFilter_;
    return *this;
}


Texture::Texture(IOpenGLWrapper& gl, const Texture::Params& params)
    : _gl(&gl),
      _handle(*_gl, 0, [](IOpenGLWrapper& gl, GLuint h) { gl.DeleteTextures(1, &h); })
{
    // clear errors so get GetError below will be accurate
    _gl->GetError();

    {
        GLuint tmpHandle;
        _gl->GenTextures(1, &tmpHandle);
        _handle.reset(tmpHandle);
    }

    if (!_handle.get())
    {
        std::stringstream ss;
        ss << "glGenTextures failed with error: " << _gl->GetError();
        throw std::runtime_error(ss.str().c_str());
    }

    // Ensure things are in a consistent state.  Use GL_TEXTURE0 for
    // the work of setting up this texture.  This will override
    // the current binding of GL_TEXTURE0
    Bind(0);

    // Set wrapping options
    _gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGlWrapMode(params.wrapModeS));	// set texture wrapping to GL_REPEAT (default wrapping method)
    _gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGlWrapMode(params.wrapModeT));

    // Set magnification and minification options
    _gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGlMinFilter(params.minFilter));
    _gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGlMagFilter(params.magFilter));
    _gl->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, params.image.Width(),
        params.image.Height(), 0, getGlFormatFromImage(params.image), GL_UNSIGNED_BYTE, params.image.Data());

    if (doesMinFilterRequireMipmap(params.minFilter))
    {
        _gl->GenerateMipmap(GL_TEXTURE_2D);
    }

    // Unbind to avoid any external calls from accidentally affecting
    // this texture.
    _gl->BindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int textureIndex) const
{
    const auto MIN_INDEX = 0;
    const auto MAX_INDEX = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1;

    if (textureIndex < MIN_INDEX || textureIndex > MAX_INDEX)
    {
        std::stringstream ss;
        ss << "Texture index must be between " << MIN_INDEX << "";
        ss << " and " << MAX_INDEX << ".  Value was: " << textureIndex << ".";
        throw std::invalid_argument(ss.str().c_str());
    }
    _gl->ActiveTexture(GL_TEXTURE0 + textureIndex);
    _gl->BindTexture(GL_TEXTURE_2D, _handle.get());
}
