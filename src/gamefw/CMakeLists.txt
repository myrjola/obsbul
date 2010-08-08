include_directories(/usr/include/bullet)

add_library(gamefw entityfactory.cpp entity.cpp fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp rendercontext.cpp renderjob.cpp)

set(SFML_LIBRARIES sfml-audio sfml-graphics sfml-window sfml-system)

target_link_libraries(gamefw glog freeimageplus ${OPENGL_LIBRARY} tinyxml
                     ${SFML_LIBRARIES} BulletDynamics ${PHYSFS_LIBRARY}
                     glm)

add_subdirectory(tests)