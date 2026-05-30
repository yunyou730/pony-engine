#include "Shader.h"
#include "GLCheck.h"
#include "Core/Log.h"
#include "VertexBuffer.h"

namespace pony {

namespace gfx {


VertexBuffer::VertexBuffer(const float *vertices,
                           int vertexCount,
                           const std::vector<gfx::VertexAttrib> &attribs)
:_vertexCount(vertexCount) {
    
    GLsizei strideFloats = 0;
    for (auto &a: attribs) {
        strideFloats += a.components;
    }
    
    GLsizei stride = strideFloats * sizeof(float);
    
    GL_CHECK(glGenVertexArrays(1, &_vao));
    GL_CHECK(glGenBuffers(1, &_vbo));
    
    GL_CHECK(glBindVertexArray(_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER,
                          (GLsizeiptr) (vertexCount * strideFloats) * sizeof(float),
                          vertices, GL_STATIC_DRAW));
    
    GLsizeiptr offset = 0;
    for (auto &a: attribs) {
        GL_CHECK(glVertexAttribPointer(a.index, a.components, GL_FLOAT, GL_FALSE,
                                       stride, (void *) offset));
        GL_CHECK(glEnableVertexAttribArray(a.index));
        offset += a.components * sizeof(float);
    }
    
    GL_CHECK(glBindVertexArray(0));
}

VertexBuffer::~VertexBuffer() {
    if (_vbo) {
        GL_CHECK(glDeleteBuffers(1, &_vbo));
        _vbo = 0;
    }
    if (_vao) {
        GL_CHECK(glDeleteVertexArrays(1, &_vao));
        _vao = 0;
    }
}

}

}
