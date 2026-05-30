#include "TestScene.h"
#include "Core/Log.h"
#include "Runtime/Renderer/Renderer.h"
#include "Runtime/Renderer/Mesh.h"
#include "Runtime/Graphics/Shader.h"

#include <glm/glm.hpp>

namespace pony {

    static const char *VERT_SRC = R"(#version 300 es
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 vColor;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            vColor = aColor;
        }
    )";

    static const char *FRAG_SRC = R"(#version 300 es
        precision mediump float;
        in vec3 vColor;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(vColor, 1.0);
        }
    )";

    TestScene::TestScene(const std::string &rootPath, int width, int height)
            : Scene(rootPath, width, height) {

    }

    TestScene::~TestScene() {

    }

    void TestScene::onStart() {
        // create shader
        _shaderId = getRenderer()->createShader(VERT_SRC, FRAG_SRC);
        pony::Log::i("TestScene: created shader id=%d", _shaderId);

        // create mesh. position x3, color x3
        float vertices[] = {
                0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        std::vector<gfx::VertexAttrib> attribs = {
                gfx::VertexAttrib(0, 3),  // location=0, vec3 (position)
                gfx::VertexAttrib(1, 3),  // location=1, vec3 (color)
        };
        _meshId = getRenderer()->createMesh(vertices, 3, attribs);

        pony::Log::i("TestScene: created mesh id=%d", _meshId);

        glm::vec3 a(1.0f,2.0f,3.0f);
        glm::vec3 b(3.0f,4.0f,5.0f);
        float res = glm::dot(a,b);
        pony::Log::i("glm dot %.3f\n",res);
    }

    void TestScene::onDestroy() {

    }

    void TestScene::onUpdate(float deltaTime) {

    }

    void TestScene::onRender() {
        getRenderer()->drawMesh(_meshId, _shaderId);
    }
}
