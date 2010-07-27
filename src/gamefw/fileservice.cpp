/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include <fstream>

#include <FreeImagePlus.h>

#include "fileservice.h"

using namespace std;

FileService::FileService()
{
    FreeImage_Initialise( 0 );
}

FileService::~FileService()
{
    FreeImage_DeInitialise();
}

const char* FileNotFoundException::what() const throw()
{
    return "File not found.";
}

char* FileService::fileToBuffer( string filename )
{
    ifstream file( filename.c_str(), ios::in | ios::ate );
    ifstream::pos_type size;
    char* buffer;

    if ( file.is_open() ) {
        size = file.tellg();
        // The +1 in size fixes valgrind invalid read and write errors. Probably
        // missing the '\0'.
        buffer = new char[size + ( ifstream::pos_type ) 1];
        file.seekg( 0, ios::beg );
        file.read( buffer, size );
        file.close();

        buffer[size] = '\0'; // Delete EOF.

        DLOG( INFO ) << filename << " loaded into buffer.";

        return buffer;
    }

    DLOG( ERROR ) << filename << " not found.";
    throw FileNotFoundException();
}

GLuint FileService::makeTexture( string name )
{
    fipImage image = readImage( name );

    assert( image.isValid() );

    int width = image.getWidth();
    int height = image.getHeight();
    void* pixels = (void*) image.accessPixels();
    GLuint texture;

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D(
        GL_TEXTURE_2D, 0,           // target, level
        GL_RGB8,                    // internal format
        width, height, 0,           // width, height, border
        GL_BGR, GL_UNSIGNED_BYTE,   // external format, type
        pixels                      // pixels
    );

    image.clear();
    delete image;

    return texture;

}

const fipImage& FileService::readImage( string name )
{
    string filename = "assets/images/" + name + ".png";
    fipImage* image = new fipImage;

    if ( !image->load( filename.c_str() ) ) {
        DLOG( ERROR ) << filename << " not found.";
        throw FileNotFoundException();
    }
    image->convertTo24Bits();

    DLOG( INFO ) << filename << " loaded to texture.";
    return *image;
}
