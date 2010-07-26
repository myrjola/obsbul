add_executable(testgamefw testgamefw.cpp testfileservice.cpp)

target_link_libraries(testgamefw gamefw UnitTest++ boost_filesystem)

add_test(testGameFW testgamefw)