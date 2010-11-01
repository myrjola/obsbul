# - Try to find FreeImagePlus
#
# Defines:
#  FreeImagePlus_FOUND
#  FreeImagePlus_INCLUDE_DIRS
#  FreeImagePlus_LIBRARIES

# Dependencies
find_package(FreeImage REQUIRED)

# Include dir
find_path(FreeImagePlus_INCLUDE_DIR
  NAMES FreeImagePlus.h
  PATHS ${FreeImagePlus_PKGCONF_INCLUDE_DIRS}
)

# Library
find_library(FreeImagePlus_LIBRARY
  NAMES freeimageplus FreeImagePlus
  PATHS ${FreeImagePlus_PKGCONF_LIBRARY_DIRS}
)

set(FreeImagePlus_INCLUDE_DIRS FreeImagePlus_INCLUDE_DIR ${FreeImage_INCLUDE_DIRS})
set(FreeImagePlus_LIBRARIES FreeImagePlus_LIBRARY ${FreeImage_LIBRARY})