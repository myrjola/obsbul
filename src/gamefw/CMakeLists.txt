set(GAMEFW_SRCS icontroller.cpp entityfactory.cpp entity.cpp fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp renderer.cpp renderjob.cpp)
set(GAMEFW_HDRS icontroller.h entityfactory.h entity.h fileservice.h locator.h shaderprogram.h shaderfactory.h game.h gamestate.h renderer.h renderjob.h gamefw.h)

add_library(gamefw ${GAMEFW_SRCS} ${GAMEFW_HDRS})

target_link_libraries(gamefw objfile ${OPENGL_LIBRARY} ${TinyXML_LIBRARIES}
                     ${SFML2_LIBRARIES} ${PHYSFS_LIBRARY}
                     ${GLEW_LIBRARIES} ${FreeImagePlus_LIBRARIES})

add_subdirectory(tests)
add_subdirectory(convenience)