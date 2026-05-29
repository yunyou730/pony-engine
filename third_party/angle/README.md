# ANGLE — OpenGL ES on Desktop

ANGLE (Almost Native Graphics Layer Engine) 将 OpenGL ES API 翻译为桌面端原生图形 API。

## 版本支持

| GLES 版本 | 支持状态 |
|-----------|---------|
| OpenGL ES 2.0 | 完全支持 |
| OpenGL ES 3.0 | 完全支持 |
| OpenGL ES 3.1 | 完全支持 |
| OpenGL ES 3.2 | 大部分支持 |

本项目使用 GLES 3.0。

## 后端

| 平台 | 翻译目标 |
|------|---------|
| Windows | Direct3D 11 或 Vulkan |
| macOS | Metal |

## 目录结构

```
angle/
├── include/              ← 头文件（全平台通用）
│   ├── GLES3/gl3.h
│   ├── GLES3/gl3platform.h
│   ├── EGL/egl.h
│   ├── EGL/eglplatform.h
│   └── KHR/khrplatform.h
├── win64/                ← Windows x64 二进制
│   ├── libEGL.dll
│   ├── libEGL.dll.lib
│   ├── libGLESv2.dll
│   └── libGLESv2.dll.lib
└── macos/                ← macOS 二进制
    ├── libEGL.dylib
    └── libGLESv2.dylib
```

## 获取方式

### 预编译下载

- https://github.com/nicedoc/angle-prebuilt/releases

### 从 Chrome 提取（仅 Windows）

Chrome 自带 ANGLE，但只有 DLL 没有 .lib 和头文件：
```
C:\Program Files\Google\Chrome\Application\<version>\
  - libEGL.dll
  - libGLESv2.dll
```

头文件和 .lib 需要从 ANGLE 源码或预编译包获取。

### 从源码构建

```bash
git clone https://chromium.googlesource.com/angle/angle
# 按官方文档构建（依赖 depot_tools，构建过程较重）
```

## 注意

- angle/ 目录不走 git submodule（ANGLE 源码巨大且构建复杂）
- 二进制文件直接提交到仓库，或放到 Git LFS / Release 里
- 头文件全平台通用，只需要维护一份
