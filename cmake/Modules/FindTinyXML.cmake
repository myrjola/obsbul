# - Try to find TinyXML
#
# Defines:
#  TinyXML_FOUND
#  TinyXML_INCLUDE_DIRS
#  TinyXML_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(TinyXML_INCLUDE_DIR
  NAMES tinyxml.h
)

# Library
find_library(TinyXML_LIBRARY
  NAMES tinyxml
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(TinyXML_PROCESS_INCLUDES TinyXML_INCLUDE_DIR)
set(TinyXML_PROCESS_LIBS TinyXML_LIBRARY)
libfind_process(TinyXML)