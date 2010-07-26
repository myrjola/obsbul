/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include <string>

#include <GL/gl.h>

using namespace std;

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

class ShaderProgram
{
    public:
        // Compiles and links a shader program from the given source code.
        ShaderProgram(string vertex_source,
                      string geometry_source,
                      string fragment_source);

        // Returns shader_program.
        GLuint getProgram();

        // Detaches shaders and deletes them and the program.
        ~ShaderProgram();

    private:
        GLuint compile_shader(GLenum type, char* defines, char* source);

        void link_program();
        
        GLuint vertex_shader, geometry_shader, fragment_shader, shader_program;

};

#endif // SHADERPROGRAM_H
