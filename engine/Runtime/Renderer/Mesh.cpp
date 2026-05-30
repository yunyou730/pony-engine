#include "Mesh.h"
#include "Core/Log.h"

namespace pony {

Mesh::Mesh(const float *vertices,
           int vertexCount,
           const std::vector<gfx::VertexAttrib> &attribs) {
    _vertexBuffer = std::make_unique<gfx::VertexBuffer>(vertices,vertexCount,attribs);
}

Mesh::~Mesh() {
    pony::Log::i("Mesh::~Mesh()");

}
}
