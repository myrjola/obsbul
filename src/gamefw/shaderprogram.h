/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include <set>

#include <GL/gl.h>

using namespace std;

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

class ShaderProgram
{
public:
    // Compiles and links a shader program from the given source code.
    ShaderProgram(char* vertex_source,
                  char* geometry_source,
                  char* fragment_source,
                  set<string>& defines);

    // Returns shader_program.
    GLuint getProgram();

    // Returns vector of defines used when compiling the program.
    set<string>& getDefines();
    
    // Reloads shaders even in the middle of the game.
    void reload();

    // Detaches shaders and deletes them and the program.
    ~ShaderProgram();

private:
    GLuint compileShader(GLenum type, char** defines, char* source);

    void linkProgram();

    GLuint vertex_shader, geometry_shader, fragment_shader, shader_program;

    set<string> defines;

    

};

#endif // SHADERPROGRAM_H
