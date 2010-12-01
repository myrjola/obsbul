#include "../common.h"
#include "../ogl.h"

#include <set>

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

namespace gamefw {

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
     * @param defines Set of defines used when compiling. Empty set not allowed.
     */
    ShaderProgram(char const* vertex_source,
                  char const* geometry_source,
                  char const* fragment_source,
                  const set<string>& defines);

    /**
     * @return The object ID of the shader program.
     */
    uint getProgramID() const;

    /**
     * @return The defines used in the compilation of the program.
     */
    const std::set< string >& getDefines() const;

    /**
     * @brief Reloads shader program with the new source files.
     */
    void reloadProgram(const char* vertex_source, const char* geometry_source,
                       const char* fragment_source);
    
    /**
     * Detaches shaders and deletes them and the program.
     */
    ~ShaderProgram();

private:

    void deleteShaders();
    
    GLuint compileShader(GLenum type, const std::set< string >& defines, const char* source);

    void makeProgram(const GLuint program_id);

    void logErrors(GLuint object_id, PFNGLGETSHADERIVPROC shader_iv,
                   PFNGLGETSHADERINFOLOGPROC shader_infolog);

    GLuint m_vertex_shader, m_geometry_shader, m_fragment_shader, m_program_id;

    char const* m_vertex_source, *m_geometry_source, *m_fragment_source;

    set<string> m_defines;
};

}

#endif // SHADERPROGRAM_H
