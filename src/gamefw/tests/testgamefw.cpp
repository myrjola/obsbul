/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include <UnitTest++.h>
#include <glog/logging.h>

#include <GL/gl.h>

#include <physfs.h>

#include "../gamefw.h"

using namespace gamefw;

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);

    PHYSFS_init(argv[0]);
    DLOG(INFO) << PHYSFS_getBaseDir();

    Game* game = new Game();
    GLint maxbuffers;
    //     while (true)
    //         game->update();
    return UnitTest::RunAllTests();
}
