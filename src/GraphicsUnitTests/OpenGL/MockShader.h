#pragma once
#include <gmock/gmock.h>

#include <jkengine/Graphics/OpenGL/IShader.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockShader : public Graphics::OpenGL::IShader {
public:
    MOCK_METHOD(GLuint, Handle, (), (const override));
};
#pragma clang diagnostic pop
