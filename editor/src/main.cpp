#include <SDL3/SDL.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "Core/Log.h"
#include "Runtime/Scene/TestScene.h"

#include <memory>

int main(int argc, char *argv[]) {
    // 强制 SDL 使用 EGL（而不是 WGL），这样才能走 ANGLE
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        pony::Log::e("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // 请求 OpenGL ES 3.0 context（桌面上由 ANGLE 提供）
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    SDL_Window *window = SDL_CreateWindow("Pony Editor", 1440, 900,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        pony::Log::e("SDL_CreateWindow failed: %s", SDL_GetError());
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        pony::Log::e("SDL_GL_CreateContext failed: %s", SDL_GetError());
        return 1;
    }
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    pony::Log::i("GL_VENDOR: %s", glGetString(GL_VENDOR));
    pony::Log::i("GL_RENDERER: %s", glGetString(GL_RENDERER));
    pony::Log::i("GL_VERSION: %s", glGetString(GL_VERSION));

    // ImGui 初始化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    pony::Log::i("Pony Editor started");

    // 创建 TestScene
    int w, h;
    SDL_GetWindowSizeInPixels(window, &w, &h);
    auto scene = std::make_unique<pony::TestScene>("", w, h);
    scene->onStart();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSizeInPixels(window, &w, &h);
                glViewport(0, 0, w, h);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // 编辑器 UI
        ImGui::Begin("Scene");
        ImGui::Text("TestScene running");
        ImGui::Text("Viewport: %d x %d", w, h);
        ImGui::End();

        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->onUpdate(0.016f);
        scene->onRender();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    scene->onDestroy();
    scene.reset();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
