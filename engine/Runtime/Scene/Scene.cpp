#include "Scene.h"
#include "Core/Log.h"
#include "Runtime/Graphics/Graphic.h"

namespace pony {
    Scene::Scene(const std::string &rootPath, int width, int height)
            : _rootPath(rootPath), _width(width), _height(height),
              _graphic(std::make_unique<Graphic>()) {
        pony::Log::i("Scene::Scene()");
    }

    Scene::~Scene() {
        pony::Log::i("Scene::~Scene()");
    }
}