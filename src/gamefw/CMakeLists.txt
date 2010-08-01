include_directories(/usr/include/bullet)

add_library(gamefw fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp rendercontext.cpp renderjob.cpp)

set(SFML_LIBRARIES sfml-audio sfml-graphics sfml-window sfml-system)

target_link_libraries(gamefw glog freeimageplus ${OPENGL_gl_LIBRARY}
                        ${SFML_LIBRARIES} BulletDynamics ${PHYSFS_LIBRARY})

add_subdirectory(tests)