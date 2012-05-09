
set(testgamefw_SRCS testentityfactory.cpp testshaderfactory.cpp
    testgamefw.cpp testfileservice.cpp)

if(UnitTest++_FOUND)
    add_executable(testgamefw ${testgamefw_SRCS})
    target_link_libraries(testgamefw gamefw ${Boost_LIBRARIES}
                        ${UnitTest++_LIBRARIES})
endif(UnitTest++_FOUND)

add_executable(visualtest visualtest.cpp)
target_link_libraries(visualtest gamefw_convenience)

add_test(testGameFW testgamefw)
