# ANGLE - OpenGL ES on Desktop

ANGLE translates OpenGL ES 3.0 calls to DirectX (Windows) or Metal (Mac).

## How to get the binaries

### Windows

Download from Chrome's ANGLE build:
1. Go to: https://github.com/nicedoc/angle-prebuilt/releases
   Or build from source: https://chromium.googlesource.com/angle/angle
2. You need these files in `lib/`:
   - libEGL.dll
   - libEGL.dll.lib (import library)
   - libGLESv2.dll
   - libGLESv2.dll.lib (import library)
3. You need these headers in `include/`:
   - GLES3/gl3.h
   - GLES3/gl3platform.h
   - EGL/egl.h
   - EGL/eglplatform.h
   - KHR/khrplatform.h

### macOS

Same structure but with .dylib instead of .dll:
   - lib/libEGL.dylib
   - lib/libGLESv2.dylib

### Quick option: grab from Chrome installation

On Windows, Chrome ships with ANGLE. You can copy from:
  C:\Program Files\Google\Chrome\Application\<version>\
  - libEGL.dll
  - libGLESv2.dll

But you still need the .lib import libraries and headers from the ANGLE source.
