#pragma once

#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "IOpenGLWrapper.h"
#include "OpenGLHelpers.h"
#include "UniqueHandle.h"

namespace JkEng::Graphics::OpenGL
{
    template<typename TVertex>
    class VertexArray final
    {
    private:
        class Attribute
        {
        public:
            Attribute(GLuint index, GLint size, uintptr_t offset)
                : _index(index), _size(size), _offsetBytes(offset)
            {
                if (_size < 1 || _size > 4)
                {
                    std::stringstream ss;
                    ss << "Invalid attribute size (" << _size << ") must be 1, 2, 3 or 4.";
                    throw std::invalid_argument(ss.str().c_str());
                }
            }

            Attribute(const Attribute& other) = default;
            Attribute& operator=(const Attribute&) = default;

            inline GLuint Index() const { return _index; }
            inline GLint Size() const { return _size; }
            inline uintptr_t OffsetBytes() const { return _offsetBytes; }

        private:
            GLuint _index;
            GLint _size;
            uintptr_t _offsetBytes;
        };

    public:
        class Params
        {
        public:
            Params(std::vector<TVertex> vertices)
                : _vertices(std::move(vertices))
            {

            }

            Params& TriangleElementIndices(std::vector<GLuint> triangleElementIndices)
            {
                if (triangleElementIndices.size() % 3 != 0)
                {
                    throw std::invalid_argument("Count of TriangleElementIndices must be a multiple of 3.");
                }
                _triangleElementIndices = std::move(triangleElementIndices);
                return *this;
            }
    
            Params& AddAttribute(GLint size)
            {
                // We require attributes to be added in order so the index of this
                // attribute is always equal to the size of the array before
                // adding it
                auto index = _vertexAttributes.size();

                // We currently only support float values so the offset of this
                // attribute can be calculated directly from the size of previously
                // added attributes
                auto offset = std::accumulate(
                    _vertexAttributes.begin(),
                    _vertexAttributes.end(),
                    GLsizei(0),
                    [](const GLsizei sum, const Attribute &existingAttribute)
                    {
                        return sum + existingAttribute.Size() * sizeof(GLfloat);
                    });

                _vertexAttributes.push_back(Attribute(index, size, offset));
                return *this;
            }

            Params(const Params&) = delete;
            Params& operator=(const Params&) = delete;

            inline const std::vector<TVertex>& Vertices() const { return _vertices; }
            inline const std::vector<GLuint>& TriangleElementIndices() const { return _triangleElementIndices; }
            inline const std::vector<Attribute>& Attributes() const { return _vertexAttributes; }

        private:
            std::vector<TVertex> _vertices;
            std::vector<GLuint> _triangleElementIndices;
            std::vector<Attribute> _vertexAttributes;
            GLenum _usage;
        };

        VertexArray(IOpenGLWrapper& gl, const Params& params)
            : _gl(&gl),
              _drawElements(false),
              _vertexBuffer(*_gl, 0, [](IOpenGLWrapper& gl, GLuint h) { gl.DeleteBuffers(1, &h); }),
              _elementBuffer(*_gl, 0, [](IOpenGLWrapper& gl, GLuint h) { gl.DeleteBuffers(1, &h); }),
              _vertexAttributes(params.Attributes())
        {
            if (_vertexAttributes.empty())
            {
                throw std::invalid_argument(
                    "At least one vertex attribute must be provided");
            }

            // clear errors so get GetError below will be accurate
            _gl->GetError();

            SetupVertexBuffer(params.Vertices());

            if (!params.TriangleElementIndices().empty())
            {
                SetupForDrawingElements(params.TriangleElementIndices());
            }
            else
            {
                SetupForDrawingArrays(params.Vertices().size());
            }
        
            _gl->BindBuffer(GL_ARRAY_BUFFER, 0); 
            _gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

            ThrowIfOpenGlError(*_gl, "VertexArray Constructor");
        }

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&&) = default;
        VertexArray& operator=(VertexArray&&) = default;

        void Draw()
        {
            // TODO: bind buffers
        
            if (_drawElements)
            {

                BindVertexArrayAndEnableVertexAttributes();
                _gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer.get());
                _gl->DrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, 0);
            }
            else
            {
                BindVertexArrayAndEnableVertexAttributes();
                _gl->DrawArrays(GL_TRIANGLES, 0, _drawCount);
            }
        }

    private:
        IOpenGLWrapper* _gl;
        bool _drawElements;
        GLsizei _drawCount;

        typedef UniqueHandle<std::function<void (IOpenGLWrapper&, GLuint)>> UniqueVertexArrayHandle;
        typedef UniqueHandle<std::function<void (IOpenGLWrapper&, GLuint)>> UniqueBufferHandle;

        UniqueBufferHandle _vertexBuffer;
        UniqueBufferHandle _elementBuffer;
        std::vector<Attribute> _vertexAttributes;

        void SetupVertexBuffer(const std::vector<TVertex>& vertices)
        {
            {
                GLuint tmpHandle = 0;
                _gl->GenBuffers(1, &tmpHandle);
                ThrowIfInvalidHandle(tmpHandle, "GenBuffers");
                _vertexBuffer.reset(tmpHandle);
            }
        
            _gl->BindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.get());
            ThrowIfOpenGlError(*_gl, "BindBuffer (GL_ARRAY_BUFFER)");

            _gl->BufferData(
                GL_ARRAY_BUFFER,
                vertices.size() * sizeof(TVertex),
                vertices.data(),
                GL_STATIC_DRAW);
            ThrowIfOpenGlError(*_gl, "BufferData (GL_ARRAY_BUFFER)");

            _gl->BindBuffer(GL_ARRAY_BUFFER, 0);
            ThrowIfOpenGlError(*_gl, "BufferData 0 (GL_ARRAY_BUFFER)");
        }

        void BindVertexArrayAndEnableVertexAttributes()
        {
            // TODO: I think I need to bind the vertex array buffer and do this during draw
            // (based on https://stackoverflow.com/questions/32161150/opengles-2-0-without-vao)

            _gl->BindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.get());
        
            for (const auto& attr : _vertexAttributes)
            {
                _gl->VertexAttribPointer(
                    attr.Index(),
                    attr.Size(),
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(TVertex),
                    reinterpret_cast<void*>(attr.OffsetBytes()));
                ThrowIfOpenGlError(*_gl, "VertexAttribPointer");

                _gl->EnableVertexAttribArray(attr.Index());
                ThrowIfOpenGlError(*_gl, "EnableVertexAttribArray");
            }
        }

        void SetupForDrawingElements(const std::vector<GLuint>& triangleElementIndices)
        {
            {
                GLuint tmpHandle = 0;
                _gl->GenBuffers(1, &tmpHandle);
                ThrowIfInvalidHandle(tmpHandle, "GenBuffers");
                _elementBuffer.reset(tmpHandle);
            }

            // Setup element indices if they were provided.
            _gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer.get());
            ThrowIfOpenGlError(*_gl, "BindBuffer (GL_ELEMENT_ARRAY_BUFFER)");

            _gl->BufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                triangleElementIndices.size() * sizeof(GLuint),
                triangleElementIndices.data(),
                GL_STATIC_DRAW);
            ThrowIfOpenGlError(*_gl, "BufferData (GL_ELEMENT_ARRAY_BUFFER)");

            _gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ThrowIfOpenGlError(*_gl, "BufferData 0 (GL_ELEMENT_ARRAY_BUFFER)");
    
            // Element indices were provided, so set a flag to use
            // glDrawElements instead of glDrawArrays.  Set the count
            // to pass to glDrawElements to the number of indices.
            _drawElements = true;
            _drawCount = triangleElementIndices.size();
        }

        void SetupForDrawingArrays(size_t vertexCount)
        {
            // If no element indices were provided the list of vertices
            // must be divisible by three because this class only draws
            // triangles and draws them using all vertices.
            if (vertexCount % 3 != 0)
            {
                throw std::invalid_argument(
                    "Size of vertex list must be divisible by 3 if no element index "
                    "list is provided.");
            }
    
            // No element indices were provided, so set a flag to use
            // glDrawArrays instead of glDrawElements.  Set the count
            // to pass to glDrawArrays to the number of vertices.
            _drawElements = false;
            _drawCount = vertexCount;
        }

        void ThrowIfInvalidHandle(GLuint h, const std::string funcName)
        {
            if (h != 0)
            {
                return;
            }

            std::stringstream ss;
            ss << funcName << " failed with error: " << _gl->GetError();
            throw std::runtime_error(ss.str().c_str());
        }
    };
}
