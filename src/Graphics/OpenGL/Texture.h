#pragma once
#include <string>

#include "../IImage.h"

#include "IOpenGLWrapper.h"
#include "UniqueHandle.h"

namespace Graphics::OpenGL
{
    class Texture final
    {
    public:
        enum class WrapMode
        {
            ClampToEdge,
            ClampToBorder,
            MirroredRepeat,
            Repeat,
        };

        enum class MinFilterMode
        {
            Nearest,
            Linear,
            NearestMipmapNearest,
            LinearMipmapNearest,
            NearestMipmapLinear,
            LinearMipmapLinear,
        };

        enum class MagFilterMode
        {
            Nearest,
            Linear,
        };

        class Params
        {
        public:
            Params(const IImage& image_);

            Params& WrapModeS(WrapMode wrapModeS_);
            Params& WrapModeT(WrapMode wrapModeT_);
            Params& MinFilter(MinFilterMode minFilter_);
            Params& MagFilter(MagFilterMode magFilter_);

            Params(const Params&) = delete;
            Params& operator=(const Params&) = delete;

            const IImage& image;
            WrapMode wrapModeS;
            WrapMode wrapModeT;
            MinFilterMode minFilter;
            MagFilterMode magFilter;
        };

        Texture(IOpenGLWrapper* gl, const Params& params);

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;

        void Bind(int textureIndex) const;

    private:
        IOpenGLWrapper* _gl;
        typedef UniqueHandle<std::function<void (IOpenGLWrapper*, GLuint)>> UniqueTextureHandle;
        UniqueTextureHandle _handle;
    };
}
