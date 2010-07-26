/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#ifndef FILESERVICE_H
#define FILESERVICE_H
#include <string>
#include <exception>

#include <GL/gl.h>

using namespace std;

class FileNotFoundException: public exception
{
    public:
        virtual const char* what() const throw();
};

class FileService
{
    public:
        // Returns dynamically allocated char* to the file's contents. Used for
        // example to pass glsl shader sources to the compiler.
        char* fileToBuffer(string filename);

        // Creates an opengl texture and returns it's ID.
        GLuint makeTexture( string name);
};

#endif // FILESERVICE_H
