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
                             set< string >& defines) :
                             defines(defines), vertex_source(vertex_source),
                             geometry_source(geometry_source),
                             fragment_source(fragment_source)
{
    program_id = glCreateProgram();
    makeProgram(program_id);
}


GLuint ShaderProgram::getProgramID()
{
    return program_id;
}

ShaderProgram::~ShaderProgram()
{
    glDetachShader(program_id, vertex_shader);
    glDetachShader(program_id, fragment_shader);
    // TODO: Detach & delete geometry shader.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program_id);
}

set< string >& ShaderProgram::getDefines()
{
    return defines;
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
        size_t length = s.length() + 1;
        char line[length + 1];
        strcpy(line, s.c_str());
        compiler_input.push_back(line);
    }
    compiler_input.push_back(source);

    glShaderSource(shader, compiler_input.size(), (const GLchar**) &compiler_input[0], NULL);
    glCompileShader(shader);

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
    vertex_shader = compileShader(GL_VERTEX_SHADER, defines, vertex_source);
    fragment_shader = compileShader(GL_FRAGMENT_SHADER, defines, fragment_source);

    // TODO: Make geometry shader compilation optional.
//     geometry_shader = compileShader(GL_GEOMETRY_SHADER, defines, fragment_source);
//     glAttachShader(program_id, geometry_shader);
    
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
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