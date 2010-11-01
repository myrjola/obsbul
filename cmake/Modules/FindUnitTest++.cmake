# - Try to find UnitTest++
#
# Defines:
#  UnitTest++_FOUND
#  UnitTest++_INCLUDE_DIRS
#  UnitTest++_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(UnitTest++_INCLUDE_DIR
  NAMES UnitTest++.h
)

# Library
find_library(UnitTest++_LIBRARY
  NAMES UnitTest++
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(UnitTest++_PROCESS_INCLUDES UnitTest++_INCLUDE_DIR)
set(UnitTest++_PROCESS_LIBS UnitTest++_LIBRARY)
libfind_process(UnitTest++)