cmake_minimum_required(VERSION 3.22.1)
project(pony_engine_workspace)

# 桌面构建只编译 editor（Android 通过 Gradle 单独构建）
add_subdirectory(editor)
