/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "gamefw.h"

#include <set>

using namespace std;

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

/**
 * @brief Thrown when something goes awry in the creation of ShaderProgram.
 */
class ShaderProgramCreationError : public exception
{
public:
    /**
     * @return Announcement that something went wrong.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Abstraction of OpenGL shader program.
 */
class ShaderProgram
{
public:
    /**
     * Compiles and links a shader program from the given source code.
     *
     * @throw ShaderProgramCreationError When compile or linking errors occur.
     *
     * @param vertex_source Buffer to vertex shader source.
     * @param geometry_source Buffer to geometry shader source.
     * @param fragment_source Buffer to fragment shader source.
     * @param defines Set of defines used when compiling.
     */
    ShaderProgram(char* vertex_source,
                  char* geometry_source,
                  char* fragment_source,
                  set<string>& defines);

    /**
     * @return The object ID of the shader program.
     */
    GLuint getProgramID();

    /**
     * @return The defines used in the compilation of the program.
     */
    set<string>& getDefines();

    /**
     * Detaches shaders and deletes them and the program.
     */
    ~ShaderProgram();

private:
    GLuint compileShader(GLenum type, set< string >& defines, char* source);

    void makeProgram(GLuint program_id);

    void logErrors(GLuint object_id, PFNGLGETSHADERIVPROC shader_iv,
                   PFNGLGETSHADERINFOLOGPROC shader_infolog);

    GLuint m_vertex_shader, m_geometry_shader, m_fragment_shader, m_program_id;

    char* m_vertex_source, *m_geometry_source, *m_fragment_source;

    set<string> m_defines;
};

#endif // SHADERPROGRAM_H
