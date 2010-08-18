project(ObscureBulldozer)
cmake_minimum_required(VERSION 2.6)

enable_testing()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Boost REQUIRED)
find_package(PhysFS REQUIRED)

ADD_DEFINITIONS(-std=c++0x) #Enable C++0x mode

add_subdirectory(src)

add_subdirectory(assets)
