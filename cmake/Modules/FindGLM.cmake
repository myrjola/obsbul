# - Try to find GLM
#
# Defines:
#  GLM_FOUND
#  GLM_INCLUDE_DIRS

include(LibFindMacros)

# Include dir
find_path(GLM_INCLUDE_DIR
  NAMES glm/glm.hpp
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(GLM_PROCESS_INCLUDES GLM_INCLUDE_DIR)
libfind_process(GLM)