package com.pony.ayy;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class EngineGLSurfaceView extends GLSurfaceView {

    public EngineGLSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(3);
        setRenderer(new EngineRenderer());
    }

    private static class EngineRenderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            nativeOnSurfaceCreated();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            nativeOnSurfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            nativeOnDrawFrame();
        }
    }

    static native void nativeOnSurfaceCreated();

    static native void nativeOnSurfaceChanged(int width, int height);

    static native void nativeOnDrawFrame();
}
