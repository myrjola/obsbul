add_executable(testgamefw testentityfactory.cpp testshaderfactory.cpp testgamefw.cpp testfileservice.cpp)
target_link_libraries(testgamefw UnitTest++ gamefw)

add_executable(visualtest visualtest.cpp)
target_link_libraries(visualtest gamefw)

add_test(testGameFW testgamefw)