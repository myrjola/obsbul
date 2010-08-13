include_directories(/usr/include/bullet)

add_executable(testgamefw testentityfactory.cpp testshaderfactory.cpp testgamefw.cpp testfileservice.cpp)

target_link_libraries(testgamefw UnitTest++ boost_filesystem BulletDynamics gamefw)

add_executable(visualtest visualtest.cpp)
target_link_libraries(visualtest UnitTest++ boost_filesystem BulletDynamics gamefw)
# target_link_libraries(visualtest ${OPENGL_gl_LIBRARY} ${GLUT_glut_LIBRARY})

add_test(testGameFW testgamefw)