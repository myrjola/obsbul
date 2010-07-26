/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <iostream>
#include <fstream>
#include <cassert>

#include <glog/logging.h>

#include "fileservice.h"

using namespace std;

const char* FileNotFoundException::what() const throw()
{
    return "File not found.";
}

char* FileService::fileToBuffer ( string filename )
{
    ifstream file ( filename.c_str(), ios::in|ios::ate );
    ifstream::pos_type size;
    char* buffer;

    if ( file.is_open() ) {
        size = file.tellg();
        // The +1 in size fixes valgrind invalid read and write errors. Probably
        // missing the '\0'.
        buffer = new char[size + ( ifstream::pos_type ) 1];
        file.seekg ( 0, ios::beg );
        file.read ( buffer, size );
        file.close();

        buffer[size] = '\0'; // Delete EOF.

        DLOG(INFO) << filename << " loaded into buffer.";
        
        return buffer;
    }

    throw FileNotFoundException();
}

GLuint FileService::makeTexture ( string name )
{
    
}
