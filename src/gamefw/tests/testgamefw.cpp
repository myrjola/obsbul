#include <UnitTest++.h>


#include "../gamefw.h"
#include <physfs.h>

using namespace gamefw;

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    PHYSFS_init(argv[0]);
    
    return UnitTest::RunAllTests();
    return 0;
}
