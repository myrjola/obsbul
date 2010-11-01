include_directories(/usr/include/bullet)

if(UnitTest++_FOUND)
    add_executable(testobjfile testobjfile.cpp)
    target_link_libraries(testobjfile gamefw ${UnitTest++_LIBRARIES})
endif(UnitTest++_FOUND)
    
add_test(testObjFile testobjfile)