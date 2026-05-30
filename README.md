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

## 日常开发建议

### 通用

- **改 engine 逻辑**：直接改 `engine/`，然后在 editor 里运行验证（桌面调试效率最高）。
- **改 editor UI/交互**：改 `editor/`，同样在桌面直接运行。
- **构建目录分离**：建议固定使用以下目录名，避免不同生成器混用：
  - macOS：`editor/build-xcode`（Xcode）或 `editor/build`（命令行）
  - Windows：`editor/build-vs`（VS 工程）

### macOS

- **优先用 Xcode 调试**：断点、调用栈、GPU/性能工具更顺手。
- **ANGLE 不要频繁重编**：日常改 engine/editor 不需要动 ANGLE；只有升级 ANGLE 或要更新/修复驱动问题时，才按 README 的“从源码构建并集成”流程重新编并替换 `third_party/angle/macos/`。
- **遇到 dyld 缺库**：优先用 `otool -L third_party/angle/macos/libGLESv2.dylib` 查依赖，缺什么就把对应 dylib 放进 `third_party/angle/macos/`，再重新 build editor（会自动复制进 app bundle）。

### Windows

- **优先用 VS 调试**：直接 F5，结合图形调试工具。
- **确认 DLL 拷贝**：如果运行提示缺 `libEGL.dll/libGLESv2.dll`，检查 `pony_editor.exe` 同级目录是否已有（CMake 已配置 POST_BUILD 自动拷贝；如果你改了输出目录，可能需要重新生成/构建）。


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

推荐用 Visual Studio（直接调试），命令行也可。

#### 方式 A：CMake 生成 VS 工程

```powershell
cd editor
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64
cmake --build build-vs --config Debug
```

打开：`editor\build-vs\pony_editor.sln`。

#### 方式 B：Visual Studio 打开文件夹

直接用 VS 2022 打开 `pony-engine/editor/` 文件夹，VS 会识别 CMake 工程并提供构建/调试。

#### 运行时依赖（ANGLE DLL）

Windows 使用 ANGLE（GLES->D3D11）。运行前需要让 exe 目录能找到这些文件：

- `third_party/angle/win64/libEGL.dll`
- `third_party/angle/win64/libGLESv2.dll`
- `third_party/angle/win64/z.dll`

本项目的 CMake 已在 Windows 下配置为 **POST_BUILD 自动拷贝**这些 DLL 到 `pony_editor.exe` 同级目录。

### 构建编辑器（macOS）

推荐用 Xcode（方便调试/热重载），命令行也可。

#### 方式 A：生成 Xcode 工程（推荐）

```bash
cd editor
cmake -S . -B build-xcode -G Xcode
cmake --build build-xcode --config Debug
open build-xcode/pony_editor.xcodeproj
```

运行：在 Xcode 里选择 `pony_editor` scheme，Run。

#### 方式 B：命令行构建（Release/Debug）

```bash
cd editor
cmake -S . -B build
cmake --build build -j
open build/pony_editor.app
```

> macOS 下 editor 以 `.app` 形式输出（为了更容易携带 ANGLE 的 dylib）。

### macOS：ANGLE（GLES->Metal）从源码构建并集成

本项目在 macOS 使用 **ANGLE + Metal 后端** 提供 OpenGL ES 3.0。需要你本机先构建 ANGLE，再将产物拷贝到仓库的 `third_party/angle/macos/`。

#### 依赖

- Xcode（需要 MetalToolchain，确保 `xcrun -f metal` 能找到工具）
- depot_tools（提供 `gclient/gn`）

#### 构建步骤（示例路径）

ANGLE 源码假设在：`/Users/mi/Documents/data/mi_code/angle`

deopt_tools 在：`/Users/mi/Documents/data/mi_code/depot_tools`

1) 初始化/同步依赖（只需做一次）

```bash
export PATH="/Users/mi/Documents/data/mi_code/depot_tools:$PATH"
cd /Users/mi/Documents/data/mi_code
gclient root
gclient sync
```

2) 生成并编译 dylib

```bash
export PATH="/Users/mi/Documents/data/mi_code/third_party/ninja:/Users/mi/Documents/data/mi_code/depot_tools:$PATH"
export CHROMIUM_BUILDTOOLS_PATH="/Users/mi/Documents/data/mi_code/buildtools"
cd /Users/mi/Documents/data/mi_code/angle

gn gen out/Release --root="/Users/mi/Documents/data/mi_code" --args='is_debug=false target_cpu="arm64" angle_enable_metal=true is_component_build=true angle_enable_gl=false angle_enable_vulkan=false'

ninja -C out/Release libEGL libGLESv2
```

3) 拷贝产物到本仓库

将以下 dylib 拷到 `pony-engine/third_party/angle/macos/`：

- `libEGL.dylib`
- `libGLESv2.dylib`
- `libthird_party_abseil-cpp_absl.dylib`
- `libdawn_proc.dylib`
- `libdawn_native.dylib`
- `libdawn_platform.dylib`
- `libchrome_zlib.dylib`
- `libc++_chrome.dylib`

> 说明：这些依赖是 `otool -L libGLESv2.dylib`/`libdawn_native.dylib` 能看到的运行时依赖；缺哪个会在启动时 dyld 报 `Library not loaded: @rpath/...`。

4) Editor 工程会在构建后把这些 dylib 复制到 `.app/Contents/Frameworks`，并设置 rpath 为 `@executable_path/../Frameworks`。


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
