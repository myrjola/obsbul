# - Try to find GLEW
#
# Defines:
#  GLEW_FOUND
#  GLEW_INCLUDE_DIRS
#  GLEW_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(GLEW_INCLUDE_DIR
  NAMES GL/glew.h
)

# Library
find_library(GLEW_LIBRARY
  NAMES glew32s GLEW
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(GLEW_PROCESS_INCLUDES GLEW_INCLUDE_DIR)
set(GLEW_PROCESS_LIBS GLEW_LIBRARY)
libfind_process(GLEW)