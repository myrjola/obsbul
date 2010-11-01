project(ObscureBulldozer)
cmake_minimum_required(VERSION 2.6)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PhysFS REQUIRED)
find_package(FreeImagePlus REQUIRED)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS filesystem system)
set(Boost_LIBRARIES ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

set(SFML_LIBS sfml2-audio sfml2-graphics sfml2-window sfml2-system)

set (GLEW_LIBS GLEW)
if(WIN32)
    set (GLEW_LIBS glew32s)
endif(WIN32)


if(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-std=c++0x) #Enable C++0x mode for gcc.
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(src)

add_subdirectory(assets)
