# - Try to find FreeImage
#
# Defines:
#  FreeImage_FOUND
#  FreeImage_INCLUDE_DIRS
#  FreeImage_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(FreeImage_INCLUDE_DIR
  NAMES FreeImage.h
)

# Library
find_library(FreeImage_LIBRARY
  NAMES freeimage FreeImage
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(FreeImage_PROCESS_INCLUDES FreeImage_INCLUDE_DIR)
set(FreeImage_PROCESS_LIBS FreeImage_LIBRARY)
libfind_process(FreeImage)