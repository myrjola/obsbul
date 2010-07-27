/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include "shaderprogram.h"

ShaderProgram::ShaderProgram(char* vertex_source,
                             char* geometry_source,
                             char* fragment_source,
                             set< string >& defines) : defines(defines)
{

}


GLuint ShaderProgram::getProgram()
{
    return shader_program;
}

ShaderProgram::~ShaderProgram()
{

}

set< string >& ShaderProgram::getDefines()
{
    return defines;
}
