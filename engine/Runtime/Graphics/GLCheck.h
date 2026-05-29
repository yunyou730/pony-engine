#pragma once

#include <GLES3/gl3.h>

#include "Core/Log.h"

// Debug: checks glGetError after every wrapped call
// Release: zero overhead, just executes the call
#ifndef NDEBUG
#define GL_CHECK(call) do { \
    call; \
    GLenum _err = glGetError(); \
    if (_err != GL_NO_ERROR) { \
        ::pony::Log::e("GL error 0x%x at %s:%d  [%s]", _err, __FILE__, __LINE__, #call); \
    } \
} while(0)
#else
#define GL_CHECK(call) do { call; } while(0)
#endif
