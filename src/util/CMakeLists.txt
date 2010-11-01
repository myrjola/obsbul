add_library(logger logger.cpp logger.h)
add_library(objfile objfile.cpp objfile.h)
target_link_libraries(objfile logger)
add_subdirectory(tests)