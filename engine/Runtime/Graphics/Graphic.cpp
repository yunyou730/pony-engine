#include "Graphic.h"
#include "Mesh.h"
#include "Shader.h"
#include "Core/Log.h"

namespace pony {

    Graphic::Graphic() {
        pony::Log::i("Graphic::Graphic()");
    }

    Graphic::~Graphic() {
        pony::Log::i("Graphic::~Graphic(), releasing %zu meshes, %zu shaders",
             _meshes.size(), _shaders.size());
    }

    int Graphic::createMesh(const float *vertices, int vertexCount,
                            const std::vector<VertexAttrib> &attribs) {
        int id = _nextResId++;
        _meshes[id] = std::make_unique<Mesh>(vertices, vertexCount, attribs);
        return id;
    }

    Mesh *Graphic::getMesh(int id) {
        auto it = _meshes.find(id);
        return it != _meshes.end() ? it->second.get() : nullptr;
    }

    void Graphic::destroyMesh(int id) {
        _meshes.erase(id);
    }

    int Graphic::createShader(const char *vertexSrc, const char *fragmentSrc) {
        int id = _nextResId++;
        _shaders[id] = std::make_unique<Shader>(vertexSrc, fragmentSrc);
        return id;
    }

    Shader *Graphic::getShader(int id) {
        auto it = _shaders.find(id);
        return it != _shaders.end() ? it->second.get() : nullptr;
    }

    void Graphic::destroyShader(int id) {
        _shaders.erase(id);
    }

    void Graphic::drawMesh(int meshId, int shaderId) {
        Mesh *mesh = getMesh(meshId);
        Shader *shader = getShader(shaderId);
        if (!mesh || !shader || !shader->isValid()) {
            return;
        }

        shader->use();
        glBindVertexArray(mesh->vao());
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount());
        glBindVertexArray(0);
    }

}
