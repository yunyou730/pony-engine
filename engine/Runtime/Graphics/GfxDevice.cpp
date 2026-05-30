#include "GLCheck.h"
#include "GfxDevice.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Core/Log.h"

namespace pony {

namespace gfx {

GfxDevice::GfxDevice() {
    pony::Log::i("GfxDevice");
}

GfxDevice::~GfxDevice() {
    pony::Log::i("~GfxDevice");
}

void GfxDevice::drawVertexBuffer(VertexBuffer& vertexBuffer,Shader& shader){
    if(!shader.isValid()) {
        pony::Log::w("draw vertex buffer, but shader invalid!");
        return;
    }
    shader.use();
    glBindVertexArray(vertexBuffer.vao());
    glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.vertexCount());
    glBindVertexArray(0);
}

}
}
