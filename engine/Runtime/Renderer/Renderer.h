#pragma once

#include "Core/GLCompat.h"
#include "Runtime/Renderer/Mesh.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace pony {

namespace gfx {
class GfxDevice;
class Shader;
}

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    int createMesh(const float *vertices, int vertexCount,
                   const std::vector<gfx::VertexAttrib> &attribs);
    Mesh *getMesh(int id);
    void destroyMesh(int id);
    
    int createShader(const char *vertexSrc, const char *fragmentSrc);
    gfx::Shader *getShader(int id);
    void destroyShader(int id);
    
    void drawMesh(int meshId, int shaderId);
    
private:
    int _nextResId = 1;
    std::unordered_map<int, std::unique_ptr<Mesh>> _meshes;
    std::unordered_map<int, std::unique_ptr<gfx::Shader>> _shaders;
    
    
    std::unique_ptr<gfx::GfxDevice> _gfxDevice;
};

}
