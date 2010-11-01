
set(testgamefw_SRCS testentityfactory.cpp testshaderfactory.cpp
    testgamefw.cpp testfileservice.cpp)

add_executable(testgamefw ${testgamefw_SRCS})

target_link_libraries(testgamefw gamefw ${Boost_LIBRARIES}
                      ${UnitTest++_LIBRARIES})

add_executable(visualtest visualtest.cpp)
target_link_libraries(visualtest gamefw)

add_test(testGameFW testgamefw)