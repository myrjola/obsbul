# - Try to find SFML2
#
# Defines:
#  SFML2_FOUND
#  SFML2_INCLUDE_DIRS
#  SFML2_LIBRARIES

include(LibFindMacros)

# Include dir
find_path(SFML2_INCLUDE_DIR
  NAMES SFML/System.hpp
)

set(SFML_MODULES audio graphics window system)

# Library
foreach(module ${SFML_MODULES})
    find_library(SFML2_${module}_LIBRARY
    NAMES sfml2-${module}
    )
    set(SFML2_LIBS ${SFML2_LIBS} ${SFML2_${module}_LIBRARY})
endforeach(module)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(SFML2_PROCESS_INCLUDES SFML2_INCLUDE_DIR)
set(SFML2_PROCESS_LIBS SFML2_LIBS)
libfind_process(SFML2)