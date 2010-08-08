/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <iostream>
#include <fstream>
#include <stdio.h>

#include <boost/filesystem.hpp>
#include <FreeImagePlus.h>
#include <UnitTest++.h>

#include "../fileservice.h"

using namespace gamefw;

TEST(TestFileToBufferEquals)
{
    FileService fileservice;
    ofstream testdata;
    std::string filename = "temp.txt";
    testdata.open(filename.c_str());
    std::string data = "Hello,\nWorld!";
    testdata << data;
    testdata.close();
    char* buffer = fileservice.fileToBuffer(filename);
    CHECK_EQUAL(data.c_str(), buffer);
    remove(filename.c_str());
    delete[] buffer;
}

TEST(TestFileToBufferNoFile)
{
    FileService fileservice;
    char* buffer;
    CHECK_THROW(buffer = fileservice.fileToBuffer("nonexistent.txt"),
                 FileNotFoundException);
}

TEST(TestMakeTexture)
{
    FileService fileservice;

    // Create image consisting of one black pixel.
    fipImage image = fipImage(FIT_RGB16, 1, 1, 24);
    RGBQUAD black;
    black.rgbBlue = 0;
    black.rgbGreen = 0;
    black.rgbRed = 0;
    black.rgbReserved = 255;
    image.setPixelColor(0, 0, &black);
    boost::filesystem::create_directories("assets/images");
    image.save("assets/images/temp.png");
    
    GLuint texture = fileservice.makeTexture("temp");

    glDeleteTextures(1, &texture);
    remove("assets/images/temp.png");
    image.clear();
}

TEST(TestMakeTextureNoFile)
{
    FileService fileservice;
    GLuint texture;
    CHECK_THROW(texture = fileservice.makeTexture("nonexistent"),
                 FileNotFoundException);
    glDeleteTextures(1, &texture);
}

