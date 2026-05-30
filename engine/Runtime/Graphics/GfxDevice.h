#pragma once

#include "Core/GLCompat.h"
#include <vector>

namespace pony {

namespace gfx {

class VertexBuffer;
class Shader;
class GfxDevice {
public:
    GfxDevice();
    ~GfxDevice();
    
    void drawVertexBuffer(VertexBuffer& vertexBuffer,Shader& shader);
    
};

}
}
