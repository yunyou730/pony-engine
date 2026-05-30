cmake_minimum_required(VERSION 3.22.1)
project(pony_editor)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/FS /utf-8)
endif()

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
        ${ANGLE_DIR}/include
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_ES3)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# --- Engine 源文件 ---
# 使用 GLOB_RECURSE 让 Xcode/VS 的文件树自动跟随目录结构。
# 注意：新增/删除文件后需要重新 configure（CMake 重新生成工程）才能被 IDE 识别。
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
        ${ENGINE_DIR}/Core/*
        ${ENGINE_DIR}/Runtime/*
)

# --- 桌面平台 Log 实现 ---
set(PLATFORM_SOURCES
        src/platform/LogDesktop.cpp
)

# --- Editor ---
add_executable(pony_editor MACOSX_BUNDLE
        src/main.cpp
        ${ENGINE_SOURCES}
        ${PLATFORM_SOURCES}
)

# VS/Xcode 中按目录结构显示文件
source_group(TREE ${ENGINE_DIR} PREFIX "engine" FILES ${ENGINE_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "editor" FILES
        src/main.cpp
        src/platform/LogDesktop.cpp
)

target_include_directories(pony_editor PRIVATE
        ${ENGINE_DIR}
        ${ENGINE_DIR}/Runtime
        ${THIRD_PARTY_DIR}/glm
        ${ANGLE_DIR}/include
)

target_link_libraries(pony_editor PRIVATE
        SDL3::SDL3
        imgui
)

# ANGLE 链接（按平台选择预编译库目录）
if(WIN32)
    set(ANGLE_LIB_DIR ${ANGLE_DIR}/win64)
    target_link_libraries(pony_editor PRIVATE
            ${ANGLE_LIB_DIR}/libEGL.dll.lib
            ${ANGLE_LIB_DIR}/libGLESv2.dll.lib
    )
    # 编译后自动拷贝 DLL 到 exe 目录
    add_custom_command(TARGET pony_editor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libEGL.dll $<TARGET_FILE_DIR:pony_editor>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libGLESv2.dll $<TARGET_FILE_DIR:pony_editor>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/z.dll $<TARGET_FILE_DIR:pony_editor>
    )
elseif(APPLE)
    # macOS: 使用 ANGLE 动态库 (GLES->Metal)
    # third_party/angle/macos/ 里放置 libEGL.dylib / libGLESv2.dylib
    set(ANGLE_LIB_DIR ${ANGLE_DIR}/macos)

    # 链接 ANGLE 的 GLES/EGL（提供 gl* 符号实现）
    target_link_libraries(pony_editor PRIVATE
            ${ANGLE_LIB_DIR}/libEGL.dylib
            ${ANGLE_LIB_DIR}/libGLESv2.dylib
    )

    # 让可执行文件在运行时能从 app bundle 的 Frameworks 找到 dylib
    set_target_properties(pony_editor PROPERTIES
            BUILD_RPATH "@executable_path/../Frameworks"
            INSTALL_RPATH "@executable_path/../Frameworks"
    )

    # 编译后将 dylib 拷贝到 app bundle
    add_custom_command(TARGET pony_editor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libEGL.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libGLESv2.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libthird_party_abseil-cpp_absl.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libdawn_proc.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libdawn_native.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libdawn_platform.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libchrome_zlib.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANGLE_LIB_DIR}/libc++_chrome.dylib "$<TARGET_FILE_DIR:pony_editor>/../Frameworks"
    )
endif()
