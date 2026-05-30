#include "Renderer.h"
#include "Runtime/Renderer/Mesh.h"
#include "Runtime/Graphics/Shader.h"
#include "Core/Log.h"
#include "Runtime/Graphics/GfxDevice.h"

namespace pony {

Renderer::Renderer()
    :_gfxDevice(std::make_unique<gfx::GfxDevice>()) {
    pony::Log::i("Graphic::Graphic()");
}

Renderer::~Renderer() {
    pony::Log::i("Graphic::~Graphic(), releasing %zu meshes, %zu shaders",
                 _meshes.size(), _shaders.size());
    
    _shaders.clear();
    _meshes.clear();
    _gfxDevice.reset();
}

int Renderer::createMesh(const float *vertices, int vertexCount,
                         const std::vector<gfx::VertexAttrib> &attribs) {
    int id = _nextResId++;
    _meshes[id] = std::make_unique<Mesh>(vertices, vertexCount, attribs);
    return id;
}

Mesh *Renderer::getMesh(int id) {
    auto it = _meshes.find(id);
    return it != _meshes.end() ? it->second.get() : nullptr;
}

void Renderer::destroyMesh(int id) {
    _meshes.erase(id);
}

int Renderer::createShader(const char *vertexSrc, const char *fragmentSrc) {
    int id = _nextResId++;
    _shaders[id] = std::make_unique<gfx::Shader>(vertexSrc, fragmentSrc);
    return id;
}

gfx::Shader *Renderer::getShader(int id) {
    auto it = _shaders.find(id);
    return it != _shaders.end() ? it->second.get() : nullptr;
}

void Renderer::destroyShader(int id) {
    _shaders.erase(id);
}

void Renderer::drawMesh(int meshId, int shaderId) {
    
    Mesh *mesh = getMesh(meshId);
    gfx::Shader *shader = getShader(shaderId);
    if (mesh == nullptr) {
        return;
    }
    if (shader == nullptr) {
        return;
    }
    gfx::VertexBuffer* vb = mesh->getVertexBuffer();
    if(vb == nullptr) {
        return;
    }
    
    if(_gfxDevice != nullptr) {
        _gfxDevice->drawVertexBuffer(*vb,*shader);
    }
}

}
