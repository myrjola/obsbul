include_directories(/usr/include/bullet)

add_library(gamefw fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp rendercontext.cpp renderjob.cpp)

target_link_libraries(gamefw glog freeimageplus ${OPENGL_gl_LIBRARY} BulletDynamics)

add_subdirectory(tests)