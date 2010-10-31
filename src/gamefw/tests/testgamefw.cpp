#include <UnitTest++.h>
#include "../gamefw.h"
#include <physfs.h>


using namespace gamefw;

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);
    
    return UnitTest::RunAllTests();
    return 0;
}
