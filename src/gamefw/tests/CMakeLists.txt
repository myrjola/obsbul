include_directories(/usr/include/bullet)

add_executable(testgamefw testentityfactory.cpp testshaderfactory.cpp testgamefw.cpp testfileservice.cpp)

target_link_libraries(testgamefw UnitTest++ boost_filesystem BulletDynamics gamefw)

add_test(testGameFW testgamefw)