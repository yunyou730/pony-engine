#include <jni.h>
#include <GLES3/gl3.h>
#include <memory>

#include "Core/Log.h"
#include "Runtime/Scene/TestScene.h"

static std::unique_ptr<pony::TestScene> sScene;
static float sLastTime = 0.0f;

extern "C" {

JNIEXPORT void JNICALL
Java_com_pony_ayy_EngineGLSurfaceView_nativeOnSurfaceCreated(JNIEnv *env, jclass clazz) {
    pony::Log::i("nativeOnSurfaceCreated");
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

JNIEXPORT void JNICALL
Java_com_pony_ayy_EngineGLSurfaceView_nativeOnSurfaceChanged(JNIEnv *env, jclass clazz,
                                                              jint width, jint height) {
    pony::Log::i("nativeOnSurfaceChanged %dx%d", width, height);
    glViewport(0, 0, width, height);

    sScene = std::make_unique<pony::TestScene>("", width, height);
    sScene->onStart();
}

JNIEXPORT void JNICALL
Java_com_pony_ayy_EngineGLSurfaceView_nativeOnDrawFrame(JNIEnv *env, jclass clazz) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (sScene) {
        sScene->onUpdate(0.016f);
        sScene->onRender();
    }
}

} // extern "C"
