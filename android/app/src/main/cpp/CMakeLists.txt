cmake_minimum_required(VERSION 3.22.1)
project("ayy")

set(CMAKE_CXX_STANDARD 17)

# engine 源码路径
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../engine)

# engine 源文件
set(ENGINE_SOURCES
        ${ENGINE_DIR}/Runtime/Graphics/Graphic.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Mesh.cpp
        ${ENGINE_DIR}/Runtime/Graphics/Shader.cpp
        ${ENGINE_DIR}/Runtime/Scene/Scene.cpp
        ${ENGINE_DIR}/Runtime/Scene/TestScene.cpp
)

# 平台层源文件（Android 专属）
set(PLATFORM_SOURCES
        native-lib.cpp
        platform/LogAndroid.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${ENGINE_SOURCES}
        ${PLATFORM_SOURCES}
)

# 第三方库路径
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../third_party)

# 头文件搜索路径
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ENGINE_DIR}
        ${THIRD_PARTY_DIR}/glm
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        EGL
        GLESv3
)
