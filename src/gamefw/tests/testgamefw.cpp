/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <UnitTest++.h>

#include "../../common.h"

#include "../gamefw.h"
#include <GL/freeglut.h>
#include <physfs.h>

using namespace gamefw;

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    PHYSFS_init(argv[0]);
    /* Game* game = new Game(); */
    
    return UnitTest::RunAllTests();
    return 0;
}
