set(GAMEFW_SRCS entityfactory.cpp entity.cpp fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp renderer.cpp renderjob.cpp)
set(GAMEFW_HDRS entityfactory.h entity.h fileservice.h locator.h shaderprogram.h shaderfactory.h game.h gamestate.h renderer.h renderjob.h gamefw.h)

add_library(gamefw ${GAMEFW_SRCS} ${GAMEFW_HDRS})

set(SFML_LIBRARIES sfml2-audio sfml2-graphics sfml2-window sfml2-system)

target_link_libraries(gamefw freeimageplus ${OPENGL_LIBRARY} tinyxml
                     ${SFML_LIBRARIES} ${PHYSFS_LIBRARY}
                     objfile glew32s freeimage)

add_subdirectory(tests)
add_subdirectory(util)