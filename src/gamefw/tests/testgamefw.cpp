/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include <UnitTest++.h>
#include <glog/logging.h>

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    
    return UnitTest::RunAllTests();
}