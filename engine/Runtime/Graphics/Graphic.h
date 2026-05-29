#pragma once

#include <GLES3/gl3.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace pony {

    struct VertexAttrib;
    class Mesh;
    class Shader;

    class Graphic {
    public:
        Graphic();
        ~Graphic();

        int createMesh(const float *vertices, int vertexCount,
                       const std::vector<VertexAttrib> &attribs);
        Mesh *getMesh(int id);
        void destroyMesh(int id);

        int createShader(const char *vertexSrc, const char *fragmentSrc);
        Shader *getShader(int id);
        void destroyShader(int id);

        void drawMesh(int meshId, int shaderId);

    private:
        int _nextResId = 1;
        std::unordered_map<int, std::unique_ptr<Mesh>> _meshes;
        std::unordered_map<int, std::unique_ptr<Shader>> _shaders;
    };

}
