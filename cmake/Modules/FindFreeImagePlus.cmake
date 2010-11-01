# - Try to find FreeImagePlus
#
# Defines:
#  FreeImagePlus_FOUND
#  FreeImagePlus_INCLUDE_DIRS
#  FreeImagePlus_LIBRARIES

include(LibFindMacros)

# Dependencies
libfind_package(FreeImagePlus FreeImage)

# Include dir
find_path(FreeImagePlus_INCLUDE_DIR
  NAMES FreeImagePlus.h
)

# Library
find_library(FreeImagePlus_LIBRARY
  NAMES freeimageplus FreeImagePlus
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(FreeImagePlus_PROCESS_INCLUDES FreeImagePlus_INCLUDE_DIR FreeImage_INCLUDE_DIRS)
set(FreeImagePlus_PROCESS_LIBS FreeImagePlus_LIBRARY FreeImage_LIBRARIES)
libfind_process(FreeImagePlus)