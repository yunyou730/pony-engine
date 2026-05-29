# Pony Engine

跨平台渲染引擎，目标运行在 Android 车机上，同时提供 Windows / macOS 统一的可视化编辑器。

## 项目目标

- **Runtime**：轻量级 GLES 3.0 渲染器，运行在 Android 车载 HMI 上
- **Editor**：Windows / macOS 可视化场景编辑器（同一套代码）
- **统一 API**：全平台使用 OpenGL ES 3.0，渲染代码零平台差异
  - Android：系统原生提供 GLES
  - Windows：通过 [ANGLE](https://chromium.googlesource.com/angle/angle) 将 GLES 翻译为 DirectX 11
  - macOS：通过 ANGLE 将 GLES 翻译为 Metal

## 目录结构

```
pony-engine/
├── engine/              # 渲染引擎核心（纯 GLES 3.0，零平台依赖）
│   ├── Core/            # 日志、GL 兼容头文件
│   └── Runtime/
│       ├── Graphics/    # Shader、Mesh、Graphic（资源管理器）
│       └── Scene/       # Scene 基类、TestScene
│
├── editor/              # 桌面编辑器（Windows + macOS 同一份代码）
│   └── src/             # SDL3 窗口 + ImGui 界面 + engine 集成
│
├── android/             # Android 运行时（Gradle + NDK）
│   └── app/src/main/
│       ├── cpp/         # JNI 桥接 + Android 平台层
│       └── java/        # GLSurfaceView、Activity
│
└── third_party/         # 第三方依赖（git submodule + ANGLE 预编译）
    ├── SDL3/            # 窗口、输入、GL context 管理（submodule）
    ├── imgui/           # 编辑器 UI（submodule）
    ├── glm/             # 数学库（submodule）
    └── angle/           # 桌面端 GLES 实现（预编译二进制，手动放置）
```

## 架构

```
┌──────────────────────────────────────┐
│       编辑器 UI (ImGui)               │  ← 仅桌面端
├──────────────────────────────────────┤
│       引擎核心 (纯 GLES 3.0)          │  ← 全平台共享
├──────────────────────────────────────┤
│       平台层                          │
│  ┌──────────┬──────────┬───────────┐ │
│  │ Android  │ Windows  │   macOS   │ │
│  │(NDK+EGL) │ (ANGLE)  │  (ANGLE)  │ │
│  └──────────┴──────────┴───────────┘ │
└──────────────────────────────────────┘
```

engine 模块不知道自己运行在哪个平台上，只调用 GLES 3.0 API。各平台负责提供：
- GL 上下文（Android 用 EGL，桌面用 ANGLE + SDL3）
- `Log` 实现（Android 用 logcat，桌面用 printf）

## 快速开始

### 克隆仓库

```bash
git clone --recursive https://github.com/<your-username>/pony-engine.git
```

如果已经 clone 但忘了加 `--recursive`：

```bash
git submodule update --init --recursive
```

### 环境要求

| 平台 | 工具 |
|------|------|
| Windows | Visual Studio 2022（勾选"使用 C++ 的桌面开发"）、CMake 3.22+ |
| macOS | Xcode 命令行工具、CMake 3.22+ |
| Android | Android Studio、NDK 27+、CMake 3.22+（AS 自带） |

### ANGLE 配置（仅桌面端需要）

桌面编辑器需要 ANGLE 提供 GLES 3.0 环境。将预编译二进制放到 `third_party/angle/`：

```
third_party/angle/
├── include/
│   ├── GLES3/gl3.h
│   ├── GLES3/gl3platform.h
│   ├── EGL/egl.h, eglplatform.h
│   └── KHR/khrplatform.h
└── lib/
    ├── libEGL.dll + libEGL.dll.lib          (Windows)
    ├── libGLESv2.dll + libGLESv2.dll.lib    (Windows)
    ├── libEGL.dylib                          (macOS)
    └── libGLESv2.dylib                       (macOS)
```

ANGLE 预编译二进制获取方式：
- https://github.com/nicedoc/angle-prebuilt/releases
- 或从源码构建：https://chromium.googlesource.com/angle/angle

### 构建编辑器（Windows）

```powershell
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

也可以用 Visual Studio 2022 直接打开项目文件夹（文件 → 打开 → 文件夹），会自动识别 CMake 工程。

**注意：** 运行前需要将 `libEGL.dll` 和 `libGLESv2.dll` 复制到 `pony_editor.exe` 同级目录。

### 构建编辑器（macOS）

```bash
cmake -B build
cmake --build build
./build/pony_editor
```

### 构建 Android

用 Android Studio 打开 `android/` 目录，直接运行即可。Gradle 会自动调用 CMake 编译 native 代码。Android 端不需要 ANGLE，系统原生支持 GLES。

## 技术栈

| 组件 | 技术 | 作用 |
|------|------|------|
| 渲染 API | OpenGL ES 3.0 | 全平台统一图形接口 |
| 数学库 | GLM | 向量、矩阵、变换 |
| 桌面窗口 | SDL3 | 跨平台窗口 + 输入 + GL context |
| 编辑器 UI | Dear ImGui | 即时模式 GUI |
| 桌面端 GLES | ANGLE | 将 GLES 翻译为 D3D11 / Metal |
| Android 壳 | Gradle + NDK | 构建系统 + JNI 桥接 |
| 构建系统 | CMake | 编辑器和 Android native 共用 |

## License

MIT
