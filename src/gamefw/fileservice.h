/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#ifndef FILESERVICE_H
#define FILESERVICE_H
#include <string>
#include <GL/gl.h>

using namespace std;

class FileService
{
    public:
        // Returns malloc'd char* to the file's contents. Used for example to
        // pass glsl shader sources to the compiler.
        char* fileToBuffer(string filename);

        // Creates an opengl texture and returns its ID.
        GLuint makeTexture(string filename);
};

#endif // FILESERVICE_H
