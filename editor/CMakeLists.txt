cmake_minimum_required(VERSION 3.22.1)
project(pony_editor)

set(CMAKE_CXX_STANDARD 17)

# 路径定义
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../engine)
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party)

# --- SDL3 (配置为使用 ANGLE 的 GLES 后端) ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES ON CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_DIR}/SDL3 SDL3_build)

# --- ANGLE ---
# ANGLE 提供 libEGL.dll + libGLESv2.dll（预编译二进制）
# 从 third_party/angle/lib 下加载
set(ANGLE_DIR ${THIRD_PARTY_DIR}/angle)

# --- ImGui ---
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_ES3)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# --- Engine 源文件 ---
set(ENGINE_SOURCES
        ${ENGINE_DIR}/Runtime/Graphics/Graphic.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Mesh.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Shader.cpp
        ${ENGINE_DIR}/Runtime/Scene/Scene.cpp
        ${ENGINE_DIR}/Runtime/Scene/TestScene.cpp
)

# --- 桌面平台 Log 实现 ---
set(PLATFORM_SOURCES
        src/platform/LogDesktop.cpp
)

# --- Editor ---
add_executable(pony_editor
        src/main.cpp
        ${ENGINE_SOURCES}
        ${PLATFORM_SOURCES}
)

target_include_directories(pony_editor PRIVATE
        ${ENGINE_DIR}
        ${THIRD_PARTY_DIR}/glm
        ${ANGLE_DIR}/include
)

target_link_libraries(pony_editor PRIVATE
        SDL3::SDL3
        imgui
)

# ANGLE 链接（预编译库）
if(WIN32)
    target_link_libraries(pony_editor PRIVATE
            ${ANGLE_DIR}/lib/libEGL.dll.lib
            ${ANGLE_DIR}/lib/libGLESv2.dll.lib
    )
elseif(APPLE)
    target_link_libraries(pony_editor PRIVATE
            ${ANGLE_DIR}/lib/libEGL.dylib
            ${ANGLE_DIR}/lib/libGLESv2.dylib
    )
endif()
