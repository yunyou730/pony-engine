#pragma once

#include "Core/GLCompat.h"
#include <vector>

namespace pony {

    struct VertexAttrib {
        GLuint index;       // shader location
        GLint components;   // 2=vec2, 3=vec3, 4=vec4

        VertexAttrib(GLuint idx, GLint comp) : index(idx), components(comp) {}
    };

    class Mesh {
    public:
        Mesh(const float *vertices, int vertexCount, const std::vector<VertexAttrib> &attribs);

        ~Mesh();

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;

        GLuint vao() const { return _vao; }

        int vertexCount() const { return _vertexCount; }

    private:
        GLuint _vao = 0;
        GLuint _vbo = 0;
        int _vertexCount = 0;
    };

}
