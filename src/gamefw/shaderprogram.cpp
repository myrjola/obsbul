/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "shaderprogram.h"

const char* ShaderProgramCreationError::what() const throw()
{
    return "Error when creating shader program.";
}

ShaderProgram::ShaderProgram(char* vertex_source,
                             char* geometry_source,
                             char* fragment_source,
                             set< string >& defines)
        :
        m_defines(defines), m_vertex_source(vertex_source),
        m_geometry_source(geometry_source),
        m_fragment_source(fragment_source)
{
    m_program_id = glCreateProgram();
    makeProgram(m_program_id);
}


GLuint ShaderProgram::getProgramID()
{
    return m_program_id;
}

ShaderProgram::~ShaderProgram()
{
    glDetachShader(m_program_id, m_vertex_shader);
    glDetachShader(m_program_id, m_fragment_shader);
    // TODO: Detach & delete geometry shader.
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
    glDeleteProgram(m_program_id);
}

set< string >& ShaderProgram::getDefines()
{
    return m_defines;
}

GLuint ShaderProgram::compileShader(GLenum type, set<string>& defines, char* source)
{
    GLuint shader = glCreateShader(type);

    // Create char** consisting of given defines and lastly the shader source.
    vector<char*> compiler_input;

    foreach (string define, defines) {
        string s = "#define ";
        s += define;
        s += "\n";
        int length = s.length() + 1;
        char* line = new char[length + 1];
        strcpy(line, s.c_str());
        compiler_input.push_back(line);
    }
    compiler_input.push_back(source);

    glShaderSource(shader, compiler_input.size(), (const GLchar**) &compiler_input[0], NULL);
    glCompileShader(shader);

    compiler_input.pop_back(); // Remove shader source so it isn't deleted.
    foreach (char* s, compiler_input) {
        delete [] s; // Delete allocated strings.
    }


    GLint status_ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status_ok);
    if (!status_ok) {
        logErrors(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        throw ShaderProgramCreationError();
    }
    return shader;
}

void ShaderProgram::makeProgram(GLuint program_id)
{
    m_vertex_shader = compileShader(GL_VERTEX_SHADER, m_defines, m_vertex_source);
    m_fragment_shader = compileShader(GL_FRAGMENT_SHADER, m_defines, m_fragment_source);

    // TODO: Make geometry shader compilation optional.
//     geometry_shader = compileShader(GL_GEOMETRY_SHADER, defines, fragment_source);
//     glAttachShader(program_id, geometry_shader);

    glAttachShader(program_id, m_vertex_shader);
    glAttachShader(program_id, m_fragment_shader);
    glLinkProgram(program_id);

    GLint status_ok;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status_ok);
    if (!status_ok) {
        logErrors(program_id, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program_id);
        throw ShaderProgramCreationError();
    }
}


void ShaderProgram::logErrors(GLuint object_id, PFNGLGETSHADERIVPROC shader_iv, PFNGLGETSHADERINFOLOGPROC shader_infolog)
{
    GLint log_length;

    shader_iv(object_id, GL_INFO_LOG_LENGTH, &log_length);
    char log[log_length];
    shader_infolog(object_id, log_length, NULL, log);
    DLOG(ERROR) << log;
}
