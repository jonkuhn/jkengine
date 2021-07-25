#pragma once
#include <gmock/gmock.h>

#include "Graphics/OpenGL/IOpenGLWrapper.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockOpenGLWrapper : public Graphics::OpenGL::IOpenGLWrapper {
public:
    MOCK_METHOD(GLenum, GetError, (), (override));
    MOCK_METHOD(GLuint, CreateShader, (GLenum type), (override));
    MOCK_METHOD(void, ShaderSource, (GLuint shader, GLsizei count, const GLchar** string, const GLint* length), (override));
    MOCK_METHOD(void, CompileShader, (GLuint shader), (override));
    MOCK_METHOD(void, GetShaderiv, (GLuint shader, GLenum, GLint*), (override));
    MOCK_METHOD(void, GetShaderInfoLog, (GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog), (override));
    MOCK_METHOD(void, DeleteShader, (GLuint shader), (override));
    MOCK_METHOD(GLuint, CreateProgram, (), (override));
    MOCK_METHOD(void, AttachShader, (GLuint program, GLuint shader), (override));
    MOCK_METHOD(void, DetachShader, (GLuint program, GLuint shader), (override));
    MOCK_METHOD(void, LinkProgram, (GLuint program), (override));
    MOCK_METHOD(void, GetProgramiv, (GLuint program, GLenum pname, GLint *params), (override));
    MOCK_METHOD(void, GetProgramInfoLog, (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog), (override));
    MOCK_METHOD(void, DeleteProgram, (GLuint program), (override));
    MOCK_METHOD(void, UseProgram, (GLuint program), (override));
    MOCK_METHOD(GLint, GetUniformLocation, (GLuint program, const GLchar* location), (override));
    MOCK_METHOD(void, Uniform1i, (GLint location, GLint v0), (override));
    MOCK_METHOD(void, UniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), (override));
    MOCK_METHOD(void, Uniform3fv, (GLint location, GLsizei count, const GLfloat *value), (override));
    MOCK_METHOD(void, Uniform2fv, (GLint location, GLsizei count, const GLfloat *value), (override));
    MOCK_METHOD(void, GenVertexArrays, (GLsizei n, GLuint *arrays), (override));
    MOCK_METHOD(void, DeleteVertexArrays, (GLsizei n, const GLuint *arrays), (override));
    MOCK_METHOD(void, BindVertexArray, (GLuint array), (override));
    MOCK_METHOD(void, VertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer), (override));
    MOCK_METHOD(void, EnableVertexAttribArray, (GLuint index), (override));
    MOCK_METHOD(void, GenBuffers, (GLsizei n, GLuint* buffers), (override));
    MOCK_METHOD(void, DeleteBuffers, (GLsizei n, const GLuint* buffers), (override));
    MOCK_METHOD(void, BindBuffer, (GLenum target, GLuint buffer), (override));
    MOCK_METHOD(void, BufferData, (GLenum target, GLsizeiptr size, const void* data, GLenum usage), (override));
    MOCK_METHOD(void, DrawArrays, (GLenum mode, GLint first, GLsizei count), (override));
    MOCK_METHOD(void, DrawElements, (GLenum mode, GLsizei count, GLenum type, const void* indices), (override));
    MOCK_METHOD(void, GenTextures, (GLsizei n, GLuint* textures), (override));
    MOCK_METHOD(void, DeleteTextures, (GLsizei n, const GLuint* textures), (override));
    MOCK_METHOD(void, ActiveTexture, (GLenum texture), (override));
    MOCK_METHOD(void, BindTexture, (GLenum target, GLuint texture), (override));
    MOCK_METHOD(void, TexParameteri, (GLenum target, GLenum pname, GLint param), (override));
    MOCK_METHOD(void, TexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width,
            GLsizei height, GLint border, GLenum format, GLenum type, const void * data), (override));
    MOCK_METHOD(void, GenerateMipmap, (GLenum target), (override));
    MOCK_METHOD(void, ClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha), (override));
    MOCK_METHOD(void, Clear, (GLbitfield mask), (override));
    MOCK_METHOD(void, ReadBuffer, (GLenum mode), (override));
    MOCK_METHOD(void, ReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data), (override));
    MOCK_METHOD(void, GetIntegerv, (GLenum pname, GLint* data), (override));
};
#pragma clang diagnostic pop
