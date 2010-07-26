project(ObscureBulldozer)
cmake_minimum_required(VERSION 2.6)

enable_testing()

find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(src)

add_subdirectory(assets)