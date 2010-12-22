set(GAMEFW_HDRS igameworld.h levelfile.h icontroller.h entityfactory.h entity.h fileservice.h locator.h shaderprogram.h shaderfactory.h game.h igamestate.h renderer.h renderjob.h gamefw.h)
set(GAMEFW_SRCS pointlight.cpp icontroller.cpp entityfactory.cpp entity.cpp fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp renderer.cpp renderjob.cpp igameworld.cpp levelfile.cpp )

add_library(gamefw ${GAMEFW_SRCS} ${GAMEFW_HDRS})

target_link_libraries(gamefw objfile ${OPENGL_LIBRARY} ${TinyXML_LIBRARIES}
                     ${SFML2_LIBRARIES} ${PHYSFS_LIBRARY}
                     ${GLEW_LIBRARIES} ${FreeImagePlus_LIBRARIES})

add_subdirectory(tests)
add_subdirectory(convenience)