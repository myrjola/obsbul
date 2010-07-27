/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include <map>
#include <set>

#include <GL/gl.h>
#include "shaderprogram.h"

using namespace std;

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

class ShaderFactory
{
public:
    ShaderFactory();

    // Returns a ShaderProgram made from an übershader using
    // the given defines.
    ShaderProgram& makeShader(set< string > defines);

    ~ShaderFactory();

private:
    map< string, vector<GLuint> >* define_table;
    map< GLuint, ShaderProgram >*  program_table;

    char* vertex_source, *geometry_source, *fragment_source;
};




#endif // SHADERFACTORY_H
