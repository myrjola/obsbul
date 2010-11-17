# - Try to find OIS
#
# Defines:
#  OIS_FOUND
#  OIS_INCLUDE_DIRS
#  OIS_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(OIS_INCLUDE_DIR
  NAMES OIS/OIS.h
)

# Library
find_library(OIS_LIBRARY
  NAMES OIS
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(OIS_PROCESS_INCLUDES OIS_INCLUDE_DIR)
set(OIS_PROCESS_LIBS OIS_LIBRARY)
libfind_process(OIS)