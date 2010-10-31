include_directories(/usr/include/bullet)

add_library(gamefw entityfactory.cpp entity.cpp fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp renderer.cpp renderjob.cpp)

set(SFML_LIBRARIES sfml2-audio sfml2-graphics sfml2-window sfml2-system)

target_link_libraries(gamefw glog freeimageplus ${OPENGL_LIBRARY} tinyxml
                     ${SFML_LIBRARIES} BulletDynamics ${PHYSFS_LIBRARY}
                     ${GLUT_glut_LIBRARY} png objfile)
message(${SFML_LIBRARIES})

add_subdirectory(tests)
add_subdirectory(util)