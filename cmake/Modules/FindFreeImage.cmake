# - Try to find FreeImage
#
# Defines:
#  FreeImage_FOUND
#  FreeImage_INCLUDE_DIR
#  FreeImage_LIBRARY

# Include dir
find_path(FreeImage_INCLUDE_DIR
  NAMES FreeImage.h
)

# Library
find_library(FreeImage_LIBRARY
  NAMES freeimage FreeImage
)