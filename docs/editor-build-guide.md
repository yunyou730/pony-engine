# Editor 构建指南

本文档说明如何在 Windows 和 macOS 上构建和开发 Pony Editor。

## 核心原则

- **CMakeLists.txt 是唯一的工程描述文件**
- .sln（Windows）和 .xcodeproj（macOS）是 CMake 在本地生成的产物，不上传版本库
- 新增源文件时只改 CMakeLists.txt，然后重新 configure，IDE 工程自动更新

---

## Windows（Visual Studio 2022）

### 方式 A：VS 直接打开文件夹（推荐）

```
Visual Studio → 文件 → 打开 → 文件夹 → 选择 pony-engine/
```

VS 会自动检测顶层 `CMakeLists.txt` 并开始 configure。完成后：
- 左侧解决方案资源管理器显示 CMake 目标
- 可直接编译、运行、断点调试
- 修改 CMakeLists.txt 后 VS 自动重新 configure

### 方式 B：生成 .sln 再打开

```powershell
cd D:\micode\pony-engine
cmake -B build -G "Visual Studio 17 2022" -A x64
```

然后打开 `build\pony_engine_workspace.sln`。

### 新增源文件

1. 创建新的 .h / .cpp 文件
2. 在 `editor/CMakeLists.txt`（或 `android/app/src/main/cpp/CMakeLists.txt`）的源文件列表里加上路径
3. VS 自动检测到 CMakeLists.txt 变化并重新 configure（方式 A）
4. 如果用方式 B，手动点：项目 → 配置缓存（Project → Configure Cache）

### 编译运行

- 顶部选择构建目标为 `pony_editor.exe`
- 选 Debug / Release
- F5 启动调试

### 注意事项

- 运行前确保 `libEGL.dll` 和 `libGLESv2.dll`（ANGLE）在 exe 同级目录
- 可以在 CMakeLists.txt 加 post-build 拷贝步骤自动化这件事

---

## macOS（Xcode）

### 方式 A：生成 Xcode 工程（推荐，支持 Instruments GPU 分析）

```bash
cd ~/Projects/pony-engine
cmake -B build -G Xcode
open build/pony_editor.xcodeproj
```

Xcode 打开后：
- 左上角选 scheme 为 `pony_editor`
- Cmd+R 运行，Cmd+Y 调试
- 可使用 Instruments（Metal System Trace、Time Profiler）

### 方式 B：命令行编译

```bash
cmake -B build
cmake --build build
./build/pony_editor
```

适合快速验证编译，但没有 IDE 调试和 GPU 分析能力。

### 新增源文件

1. 创建新的 .h / .cpp 文件
2. 修改 `editor/CMakeLists.txt` 的源文件列表
3. 重新生成 Xcode 工程：

```bash
cmake -B build -G Xcode
```

4. Xcode 会提示 "project file changed"，点 Revert 即可

### 注意事项

- 确保 ANGLE 的 `libEGL.dylib` 和 `libGLESv2.dylib` 在可执行文件能找到的路径
- 可以在 CMakeLists.txt 里设置 `RPATH` 解决动态库查找问题

---

## 通用：新增源文件的流程

以新增 `engine/Runtime/Graphics/Texture.h` 和 `Texture.cpp` 为例：

### 第 1 步：创建文件

在对应目录创建 .h 和 .cpp。

### 第 2 步：修改 CMakeLists.txt

编辑器用到的文件改 `editor/CMakeLists.txt`：

```cmake
set(ENGINE_SOURCES
        ${ENGINE_DIR}/Runtime/Graphics/Graphic.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Mesh.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Shader.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Texture.cpp    # ← 新增
        ${ENGINE_DIR}/Runtime/Scene/Scene.cpp
        ${ENGINE_DIR}/Runtime/Scene/TestScene.cpp
)
```

Android 端也要同步改 `android/app/src/main/cpp/CMakeLists.txt`。

### 第 3 步：重新 configure

| IDE | 操作 |
|-----|------|
| VS（文件夹模式） | 自动触发，无需手动操作 |
| VS（.sln 模式） | 项目 → 配置缓存 |
| Xcode | 重新执行 `cmake -B build -G Xcode` |
| CLion | 自动触发 |

### 第 4 步：提交

只提交 CMakeLists.txt 和新的源文件，不提交 build/ 下的任何东西。

---

## .gitignore 参考

```gitignore
# 构建产物
build/

# IDE 本地配置
.vs/
.idea/
*.xcworkspace/
xcuserdata/

# 系统文件
.DS_Store
Thumbs.db
```

---

## 常见问题

### Q: 改了 CMakeLists.txt 但 VS/Xcode 没反应？

手动触发 configure：
- VS：项目 → 删除缓存并重新配置
- Xcode：删掉 build/ 重新 `cmake -B build -G Xcode`

### Q: 能不能不手动列文件，自动收集？

可以用 `file(GLOB_RECURSE)`：

```cmake
file(GLOB_RECURSE ENGINE_SOURCES
        ${ENGINE_DIR}/Runtime/*.cpp
        ${ENGINE_DIR}/Core/*.cpp
)
```

好处：加文件不用改 CMakeLists.txt
坏处：CMake 不会自动感知新文件，需要手动 configure 一次

### Q: editor 和 android 的 CMakeLists.txt 都要改，能不能合成一个？

不能。Android 的 CMakeLists.txt 是被 Gradle 调用的，有特殊的 NDK 工具链注入。桌面端是独立的 CMake 构建。两者共享 engine 源文件路径，但构建入口必须分开。

将来可以把 engine 的文件列表抽成一个 `engine/sources.cmake`，两边 include 它，避免同步两份文件列表：

```cmake
# engine/sources.cmake
set(ENGINE_SOURCES
        ${ENGINE_DIR}/Runtime/Graphics/Graphic.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Mesh.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Shader.cpp
        ${ENGINE_DIR}/Runtime/Scene/Scene.cpp
        ${ENGINE_DIR}/Runtime/Scene/TestScene.cpp
)
```

```cmake
# 两边各自 include
include(${ENGINE_DIR}/sources.cmake)
```
