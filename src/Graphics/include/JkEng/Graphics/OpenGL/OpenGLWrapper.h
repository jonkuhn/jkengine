#pragma once
#include "IOpenGLWrapper.h"

#include <jkengine/Window/IOpenGLWindow.h>

#include <mutex>
#include <sstream>

namespace JkEng::Graphics::OpenGL
{
    class OpenGLWrapper final : public IOpenGLWrapper
    {
    public:
        OpenGLWrapper(Window::IOpenGLWindow& window)
            : _window(window)
        {

        }

        // There is need to copy or move this class.
        OpenGLWrapper(const OpenGLWrapper&) = delete;
        OpenGLWrapper& operator=(const OpenGLWrapper&) = delete;
        OpenGLWrapper(OpenGLWrapper&&) = delete;
        OpenGLWrapper& operator=(OpenGLWrapper&&) = delete;

        GLenum GetError() override
        {
            return glGetError();
        }

        GLuint CreateShader(GLenum shaderType) override
        {
            return glCreateShader(shaderType);
        }

        void ShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) override
        {
            glShaderSource(shader, count, string, length);
        }

        void CompileShader(GLuint shader) override
        {
            glCompileShader(shader);
        }

        void GetShaderiv(GLuint shader, GLenum pname, GLint* params) override
        {
            glGetShaderiv(shader, pname, params);
        }

        void GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) override
        {
            glGetShaderInfoLog(shader, maxLength, length, infoLog);
        }

        void DeleteShader(GLuint shader) override
        {
            glDeleteShader(shader);
        }

        GLuint CreateProgram() override
        {
            return glCreateProgram();
        }

        void AttachShader(GLuint program, GLuint shader) override
        {
            glAttachShader(program, shader);
        }

        void DetachShader(GLuint program, GLuint shader) override
        {
            glDetachShader(program, shader);
        }

        void LinkProgram(GLuint program) override
        {
            glLinkProgram(program);
        }

        void GetProgramiv(GLuint program, GLenum pname, GLint *params) override
        {
            glGetProgramiv(program, pname, params);
        }

        void GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) override
        {
            glGetProgramInfoLog(program, maxLength, length, infoLog);
        }

        void DeleteProgram(GLuint program) override
        {
            glDeleteProgram(program);
        }

        void UseProgram(GLuint program) override
        {
            glUseProgram(program);
        }

        GLint GetUniformLocation(GLuint program, const GLchar* location) override
        {
            return glGetUniformLocation(program, location);
        }

        void Uniform1i(GLint location, GLint v0) override
        {
            glUniform1i(location, v0);
        }

        void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) override
        {
            glUniformMatrix4fv(location, count, transpose, value);
        }

        void Uniform3fv(GLint location, GLsizei count, const GLfloat *value) override
        {
            glUniform3fv(location, count, value);
        }

        void Uniform2fv(GLint location, GLsizei count, const GLfloat *value) override
        {
            glUniform2fv(location, count, value);
        }

        void GenVertexArrays(GLsizei n, GLuint* arrays) override
        {
            glGenVertexArrays(n, arrays);
        }

        void DeleteVertexArrays(GLsizei n, const GLuint *arrays) override
        {
            glDeleteVertexArrays(n, arrays);
        }

        void BindVertexArray(GLuint array) override
        {
            glBindVertexArray(array);
        }

        void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) override
        {
            glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        }

        void EnableVertexAttribArray(GLuint index) override
        {
            glEnableVertexAttribArray(index);
        }

        void GenBuffers(GLsizei n, GLuint* buffers) override
        {
            glGenBuffers(n, buffers);
        }

        void DeleteBuffers(GLsizei n, const GLuint* buffers) override
        {
            glDeleteBuffers(n, buffers);
        }

        void BindBuffer(GLenum target, GLuint buffer) override
        {
            glBindBuffer(target, buffer);
        }

        void BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) override
        {
            glBufferData(target, size, data, usage);
        }

        void DrawArrays(GLenum mode, GLint first, GLsizei count) override
        {
            glDrawArrays(mode, first, count);
        }

        void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) override
        {
            glDrawElements(mode, count, type, indices);
        }

        void GenTextures(GLsizei n, GLuint* textures) override
        {
            glGenTextures(n, textures);
        }

        void DeleteTextures(GLsizei n, const GLuint* textures) override
        {
            glDeleteTextures(n, textures);
        }

        void ActiveTexture(GLenum texture) override
        {
            glActiveTexture(texture);
        }

        void BindTexture(GLenum target, GLuint texture) override
        {
            glBindTexture(target, texture);
        }

        void TexParameteri(GLenum target, GLenum pname, GLint param) override
        {
            glTexParameteri(target, pname, param);
        }

        void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
            GLsizei height, GLint border, GLenum format, GLenum type, const void* data) override
        {
            glTexImage2D(target, level, internalformat, width, height,
                border, format, type, data);
        }

        void GenerateMipmap(GLenum target) override
        {
            glGenerateMipmap(target);
        }

        void Enable(GLenum cap) override
        {
            glEnable(cap);
        }

        void BlendFunc(GLenum sfactor, GLenum dfactor) override
        {
            glBlendFunc(sfactor, dfactor);
        }

        void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) override
        {
            glClearColor(red, green, blue, alpha);
        }

        void Clear(GLbitfield mask) override
        {
            glClear(mask);
        }

        void ReadBuffer(GLenum mode) override
        {
            glReadBuffer(mode);
        }

        void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data) override
        {
            glReadPixels(x, y, width, height, format, type, data);
        }

        void GetIntegerv(GLenum pname, GLint* data) override
        {
            glGetIntegerv(pname, data);
        }

    private:
        // Unused, but exists to document dependence on a window to draw in
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunused-private-field"
        Window::IOpenGLWindow& _window;
        #pragma clang diagnostic pop
    };
}
