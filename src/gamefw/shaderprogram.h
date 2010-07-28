/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "gamefw.h"

#include <set>

using namespace std;

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

class ShaderProgramCreationError : public exception
{
public:
    virtual const char* what() const throw();
};

class ShaderProgram
{
public:
    // Compiles and links a shader program from the given source code.
    // Throws ShaderProgramCreationError if errors occur.
    ShaderProgram(char* vertex_source,
                  char* geometry_source,
                  char* fragment_source,
                  set<string>& defines);

    // Returns shader_program.
    GLuint getProgramID();

    // Returns vector of defines used when compiling the program.
    set<string>& getDefines();
    
    // Detaches shaders and deletes them and the program.
    ~ShaderProgram();

private:
    GLuint compileShader(GLenum type, set< string >& defines, char* source);

    void makeProgram(GLuint program_id);

    void logErrors(GLuint object_id, PFNGLGETSHADERIVPROC shader_iv,
        PFNGLGETSHADERINFOLOGPROC shader_infolog);

    GLuint vertex_shader, geometry_shader, fragment_shader, program_id;

    char* vertex_source, *geometry_source, *fragment_source;

    set<string> defines;

    

};

#endif // SHADERPROGRAM_H
