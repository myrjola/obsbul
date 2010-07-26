/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <UnitTest++.h>
#include "../fileservice.h"

TEST(TestFileToBuffer)
{
    FileService fileservice;
    ofstream testdata;
    testdata.open("temp.txt");
    std::string data = "Hello,\nWorld!";
    testdata << data;
    testdata.close();
    char* buffer = fileservice.fileToBuffer("temp.txt");
    CHECK_EQUAL(data.c_str(), buffer);
    remove(data.c_str());
    delete[] buffer;
}
