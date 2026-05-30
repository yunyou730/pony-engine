#pragma once

#include <vector>
#include "Runtime/Graphics/VertexBuffer.h"

namespace pony {

/*
 目前是对 VertexBuffer 的封装.
 将来要做， 多个 submesh , LOD 等等
 */
class Mesh {
public:
    Mesh(const float *vertices, int vertexCount, const std::vector<gfx::VertexAttrib> &attribs);
    ~Mesh();
    
    gfx::VertexBuffer* getVertexBuffer() { return _vertexBuffer.get();}
    
private:
    std::unique_ptr<gfx::VertexBuffer> _vertexBuffer;
};

}
