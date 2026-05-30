#include "Scene.h"
#include "Core/Log.h"
#include "Runtime/Renderer/Renderer.h"

namespace pony {
    Scene::Scene(const std::string &rootPath, int width, int height)
            : _rootPath(rootPath), _width(width), _height(height),
              _renderer(std::make_unique<Renderer>()) {
        pony::Log::i("Scene::Scene()");
    }

    Scene::~Scene() {
        pony::Log::i("Scene::~Scene()");
    }
}
