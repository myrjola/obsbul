include_directories(/usr/include/bullet)

add_executable(testobjfile testobjfile.cpp)
target_link_libraries(testobjfile gamefw ${UnitTest++_LIBRARIES})

add_test(testObjFile testobjfile)