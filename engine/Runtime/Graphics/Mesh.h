#pragma once

#include <GLES3/gl3.h>
#include <vector>

namespace pony {

    // 每个Vertex的属性.  每个 Mesh会有多个VertexAttrib,每个attribute 有不同的 semantic
    // 比如,有 uv1,uv2, color1,color2, bone weight 等等. Mesh构造时可以灵活设置每个 attribute的语义
    struct VertexAttrib {
        GLuint index;       // shader location
        GLint components;   // 2=vec2, 3=vec3, 4=vec4
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
