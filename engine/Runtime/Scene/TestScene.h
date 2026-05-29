#pragma once

#include "Scene.h"


#include <string>

namespace pony {
    class TestScene : public Scene {
    public:
        explicit TestScene(const std::string &rootPath, int width, int height);

        virtual ~TestScene();

        virtual void onStart();

        virtual void onDestroy();

        virtual void onUpdate(float deltaTime);

        virtual void onRender();

    private:
        int _meshId = 0;
        int _shaderId = 0;
    };
}
