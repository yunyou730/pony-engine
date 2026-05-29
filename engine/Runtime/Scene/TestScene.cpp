#include "TestScene.h"
#include "Core/Log.h"
#include "Runtime/Graphics/Graphic.h"
#include "Runtime/Graphics/Mesh.h"
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
        _shaderId = getGraphic()->createShader(VERT_SRC, FRAG_SRC);
        pony::Log::i("TestScene: created shader id=%d", _shaderId);

        // create mesh. position x3, color x3
        float vertices[] = {
                0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        std::vector<VertexAttrib> attribs = {
                VertexAttrib(0, 3),  // location=0, vec3 (position)
                VertexAttrib(1, 3),  // location=1, vec3 (color)
        };
        _meshId = getGraphic()->createMesh(vertices, 3, attribs);

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
        getGraphic()->drawMesh(_meshId, _shaderId);
    }
}