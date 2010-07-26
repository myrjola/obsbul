/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <UnitTest++.h>
#include "../fileservice.h"

TEST(TestFileToBufferEquals)
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

TEST(TestFileToBufferNoFile)
{
    FileService fileservice;
    try {
        char* buffer = fileservice.fileToBuffer("nonexistent.txt");
        CHECK(false); // Exception should have been thrown.
    } catch (FileNotFoundException& e) {}
}
