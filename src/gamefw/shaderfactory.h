/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include <vector>
#include <map>

#include <GL/gl.h>
#include <string>

using namespace std;

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

class ShaderFactory
{
    public:
        ShaderFactory();

        // Returns the ID of the shader object, made from an übershader using
        // the given defines.
        GLuint makeShader(vector<string> defines);

        ~ShaderFactory();

    private:
        GLuint alreadyCompiledCheck(vector<string> defines);
};

        
        

#endif // SHADERFACTORY_H
