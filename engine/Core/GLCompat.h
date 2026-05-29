#pragma once

// 所有平台统一使用 GLES3 API
// Android: 系统原生提供
// Windows/Mac: 通过 ANGLE 提供 (libEGL + libGLESv2)
#include <GLES3/gl3.h>
