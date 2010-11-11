set(GAMEFW_CONVENIENCE_SRCS defaultfirstpersoncontroller.cpp)
set(GAMEFW_CONVENIENCE_HDRS defaultfirstpersoncontroller.h)
add_library(gamefw_convenience ${GAMEFW_CONVENIENCE_HDRS}
    ${GAMEFW_CONVENIENCE_SRCS})
target_link_libraries(gamefw_convenience gamefw)