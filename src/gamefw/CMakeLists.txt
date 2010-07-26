include_directories(/usr/include/bullet)

add_library(gamefw fileservice.cpp locator.cpp shaderprogram.cpp shaderfactory.cpp game.cpp gamestate.cpp rendercontext.cpp renderjob.cpp)

add_subdirectory(tests)