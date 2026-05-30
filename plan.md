# Pony Engine — 迭代计划（ECS → 渲染 → Lua）

> 目标：尽快跑出一个“**能看见、能验证**”的 editor/runtime 循环，并且优先级清晰，照着做就能稳定迭代。
>
> 本计划范围：先用 C++ 把 **Transform 树 + Camera（View/Projection）+ MeshRenderer** 跑通；用现有 `TestScene` 作为集成测试场景。等画面稳定后，再引入 pack/zip 与 Lua binding。

---

## 指导原则

1. **始终保持可渲染**：每个里程碑结束时，都应该能运行并看到（或至少能明确验证）结果。
2. **边界清晰**：
   - `engine/` 只管运行时概念：World/ECS、Transform、Camera、Renderer、资源对象。
   - `editor/` 只管桌面窗口/UI，并决定运行哪个 Scene。
3. **避免过早 ECS 复杂化**：先用简单存储（每种组件一个数组 + entity->index 映射）实现闭环，后面再优化。
4. **用 TestScene 验证**：每加一个 component/system，都通过 `TestScene` 立刻验证正确性。

---

## 里程碑 0 — 可运行的基础循环（现在基本已有）

### 完成标准
- editor 能启动、清屏，每帧调用 `TestScene::onUpdate/onRender`。

### 备注
- 迭代引擎时保持 ImGui + SDL + GLES 这条路径稳定，避免“底座不稳”。

---

## 里程碑 1 — 引入 `World` + `Entity`（最小 ECS 骨架）

### 目标
有一个运行时容器，能管理实体/组件，并按顺序跑系统（systems）。

### 步骤
1. 新增模块：`engine/Runtime/ECS/`。
2. 实现：
   - `EntityId`（建议内部用整数）与 `World`。
   - `World::createEntity()` / `destroyEntity(EntityId)`。
   - 最小的合法性校验（generation 或简单 alive set）。
3. 在 `TestScene::onStart`：
   - 创建一个 `World` 实例。
   - 创建至少 2 个 entity。
4. 在 `TestScene::onUpdate(dt)`：
   - 调用 `world.update(dt)`。

### 验收
- 程序仍然能跑（画面不变也没关系）。
- 日志能证明 entity 创建/销毁逻辑正确。

---

## 里程碑 2 — Transform 组件 + Transform 树（父子层级）

### 目标
实现 Unity 风格：每个 entity 都有 `Transform`，并支持 parent/child。

### 步骤
1. 新增 `TransformComponent`：
   - local position / rotation / scale
   - parent entity
   - children 列表（或 sibling 链）
   - world matrix 缓存 + dirty 标记
2. 确保 Transform 永远存在：
   - 要么 `World::createEntity` 自动附带 Transform
   - 要么提供 `World::getTransform(e)` 并保证必定可取
3. 实现 Transform API：
   - `setParent(child, parent)`
   - `setLocalPosition/Rotation/Scale`
   - `getWorldMatrix()`
4. 实现 `TransformSystem`：
   - 按 parent->child 的拓扑顺序更新 world matrix
   - dirty 传播到子节点
5. 在 `TestScene::onStart`：
   - 创建 `root -> child -> grandchild` 的层级
   - 给每层设置不同 local transform
6. 在 `TestScene::onUpdate(dt)`：
   - 动画 root（例如缓慢旋转）

### 验收
- 打日志：child/grandchild 的 world matrix 会随 root 变化而变化。
- 暂时不需要渲染。

---

## 里程碑 3 — Camera 组件 + View/Projection

### 目标
有一个 Camera 能输出 View/Projection 矩阵，为渲染做准备。

### 步骤
1. 新增 `CameraComponent`：
   - 投影类型：perspective
   - fov, near, far
   - aspect（来自 viewport）
2. 实现相机矩阵：
   - View：由 camera Transform 计算
   - Projection：由 fov/aspect/near/far 计算
3. 决定 viewport 尺寸存放位置：
   - 最简单：`World` 存 viewport width/height，editor resize 时更新
4. editor 事件：
   - 窗口 resize 时更新 world viewport
5. 在 `TestScene::onStart`：
   - 创建 camera entity（放在合适位置看向原点）

### 验收
- 日志输出 camera 的 view/projection（或关键参数）正确。
- 暂时不需要渲染。

---

## 里程碑 4 — 资源对象：Shader / Mesh / Material（C++ 版本）

### 目标
避免把 draw 细节都写在 scene 里；scene 只负责“组装 world 和资源引用”。

### 步骤
1. 在 `engine/Runtime/Graphics/` 下增加最小资源类型：
   - `ShaderProgram`（封装 GL program id）
   - `Mesh`（VBO/VAO + 顶点布局）
   - `Material`（引用 shader + uniform 参数集合）
2. 创建接口尽量简单：
   - `Graphics::createShader(vertSrc, fragSrc)`
   - `Graphics::createMesh(vertices, ...)`
   - `Material` 能设置 model/view/proj 或 MVP
3. Shader 约定：支持 MVP
   - 顶点 shader 乘 `u_MVP` 或 `u_Model/u_View/u_Proj`

### 验收
- 能创建 Mesh + ShaderProgram，且 scene 不直接操作 gl*（或尽量少）。

---

## 里程碑 5 — MeshRenderer 组件 + RenderSystem（第一次真正画出东西）

### 目标
用 Transform + Camera 把一个 entity 渲染出来（先画三角形）。

### 步骤
1. 新增 `MeshRendererComponent`：
   - 引用 `MeshHandle`
   - 引用 `MaterialHandle`（或 shader handle）
2. 实现 `RenderSystem`：
   - 选择 active camera（默认第一个 camera 或 main tag）
   - 遍历 MeshRenderer：
     - 计算 `MVP = P * V * Model`
     - bind shader/material
     - set uniforms
     - draw mesh
3. 更新 `TestScene::onStart`：
   - 创建一个可渲染 entity（放在原点）
   - 给它挂 MeshRenderer（三角形 mesh + 基础材质）

### 验收
- 屏幕上能看到三角形。
- 改 entity transform（或动画）能看到三角形跟着动。

---

## 里程碑 6 — 多实体 + 父子层级的“可视化验证”

### 目标
用画面而不是日志验证 Transform 树正确，并验证组件分工。

### 步骤
1. 创建多个 entity：
   - 父节点：一个大三角形
   - 子节点：一个小三角形，local offset
2. 动画父节点 transform，观察子节点跟随。
3. editor 增加简单 ImGui debug 信息：
   - entity 数量
   - camera 参数

### 验收
- 子节点的运动随父节点变化（Transform 树正确的强证据）。

---

## 里程碑 7 — Scene 抽象：`BaseScene` 负责搭建 `World`

### 目标
让未来接 Lua/pack 时更顺：Scene 只负责“构建 world”，主循环统一调用 world。

### 步骤
1. 定义 `BaseScene` 职责：
   - 拥有 `World`
   - `onStart` 构建 entities/resources
   - `onUpdate` 调用 `world.update`
   - `onRender` 调用 `world.render`（或 render system）
2. `TestScene` 保持为一个纯 C++ 的具体 scene。

### 验收
- 以后要换 scene（即使只有一个）也很容易。

---

## 里程碑 8 — 为 pack/zip 做准备（先不做热更）

### 目标
把文件访问抽象出来，让资源/脚本未来既能来自“目录（editor）”也能来自“zip（android）”。

### 步骤
1. 引入 `IFileSystem`：
   - `readText(path)`, `readBytes(path)`, `exists(path)`
2. 实现：
   - `FolderFileSystem`（桌面）
   - （后续）`ZipFileSystem`（android + 桌面）
3. 把资源加载点改为使用 `IFileSystem`，不要直接读磁盘。

### 验收
- 引擎能通过 `IFileSystem` 从目录读取 shader 文本并创建 program。

---

## 里程碑 9 — Lua binding（等画面稳定后再做）

### 目标
让 Lua 像 Unity C# 一样：给 entity 挂 ScriptComponent，获得生命周期回调。

### 步骤
1. 嵌入 Lua VM（生命周期由 `World` 或 `EngineRuntime` 拥有）。
2. 新增 `ScriptComponent`：
   - lua path
   - 实例 table
3. 生命周期：
   - pack load 时创建 VM
   - scene load 时加载脚本
   - 调用 `onStart/onUpdate/onDestroy`
4. 绑定 API（最小集）：
   - Transform 的读写
   - spawn/destroy entity
   - logging

### 验收
- 一个 Lua 脚本能每帧移动 entity。

---

## 里程碑 10 — 整包 zip 切换热更（你的核心目标）

### 目标
实现“整包切换”：切换 zip 后，不重启 app 就能跑新场景/新脚本。

### 步骤
1. 定义 `manifest`（yaml/json）：entry scene + version。
2. 实现 `ZipFileSystem` 与 pack mount/unmount。
3. 实现 `Engine::switchPack(zipPath)`：
   - destroy world + Lua VM
   - mount 新 pack
   - load entry scene
4. editor 提供 “Reload Pack” 按钮：从目录或 zip 重新 mount（模拟 android 行为）。

### 验收
- 在两个 pack 之间切换，场景与脚本随之切换，无需重启。

---

## 实用迭代检查表（每加一个组件/系统就照这个做）

1. 增加 component 类型与存储
2. 增加对应 system 的更新逻辑
3. 在 `TestScene` 里最小化使用它
4. 用“可见画面”或“明确日志”验证它确实工作

---

## 建议文件落点（仅建议）

- `engine/Runtime/ECS/World.h/.cpp`
- `engine/Runtime/ECS/Entity.h`
- `engine/Runtime/ECS/Components/Transform.h`
- `engine/Runtime/ECS/Components/Camera.h`
- `engine/Runtime/ECS/Components/MeshRenderer.h`
- `engine/Runtime/ECS/Systems/TransformSystem.*`
- `engine/Runtime/ECS/Systems/RenderSystem.*`

- `engine/Runtime/Graphics/ShaderProgram.*`
- `engine/Runtime/Graphics/Mesh.*`
- `engine/Runtime/Graphics/Material.*`

- `engine/Runtime/IO/IFileSystem.*`

---

## 立即开始（从这里按顺序做就不会迷路）

先做 **里程碑 1 → 2 → 3 → 5**（按顺序）。
在 **里程碑 6** 稳定之前，不要开始 Lua/zip。
