#pragma once

#include <memory>
#include <string>

namespace pony {

    class Renderer;

    class Scene {
    public:
        explicit Scene(const std::string &rootPath, int width, int height);

        virtual ~Scene();

        virtual void onStart() = 0;

        virtual void onDestroy() = 0;

        virtual void onUpdate(float deltaTime) = 0;

        virtual void onRender() = 0;

    protected:
        int getWidth() const { return _width; }

        int getHeight() const { return _height; }

        Renderer *getRenderer() { return _renderer.get(); }

    protected:
        std::string _rootPath;
        int _width;
        int _height;

        std::unique_ptr<Renderer> _renderer;
    };
}
