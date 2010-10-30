#include "shaderprogram.h"

using namespace gamefw;

const char* ShaderProgramCreationError::what() const throw()
{
    return "Error when creating shader program.";
}

ShaderProgram::ShaderProgram(char const* vertex_source,
                             char const* geometry_source,
                             char const* fragment_source,
                             const set< string >& defines)
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
    deleteShaders();
    glDeleteProgram(m_program_id);
}

void ShaderProgram::deleteShaders()
{
    glDetachShader(m_program_id, m_vertex_shader);
    glDetachShader(m_program_id, m_fragment_shader);
    // TODO: Detach & delete geometry shader.
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
}

void ShaderProgram::reloadProgram(const char* vertex_source, const char* geometry_source, const char* fragment_source)
{
    m_vertex_source = vertex_source;
    m_geometry_source = geometry_source;
    m_fragment_source = fragment_source;
    GLuint new_program_id = glCreateProgram();
    try {
        makeProgram(new_program_id);
    } catch (ShaderProgramCreationError) {
        DLOG(ERROR) << "Failure when reloading shader program";
        return; // Don't switch to new program if creation failed.
    }
    deleteShaders();
    glDeleteProgram(m_program_id);
    m_program_id = new_program_id;
}


set< string >& ShaderProgram::getDefines()
{
    return m_defines;
}

GLuint ShaderProgram::compileShader(GLenum type, const set<string>& defines,
                                    char const* source)
{
    GLuint shader = glCreateShader(type);

    // Create char** consisting of given defines and lastly the shader source.
    vector<char const*> compiler_input;

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

    GLint status_ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status_ok);
    if (!status_ok) {
        // Output shader source with line number for convenience.
        int line_number = 1;
        stringstream shader_source;
        shader_source << '\n';

        int number_of_defines = compiler_input.size() - 1;
        DLOG(ERROR) << "Shader source:\n";
        for (int i = 0; i < number_of_defines; i++) {
            shader_source << line_number++ << '\t';
            shader_source << compiler_input[i];
        }
        istringstream rest_of_source(string(compiler_input[compiler_input.size() - 1]));
        string line;
        while (!rest_of_source.eof()) {
            getline(rest_of_source, line);
            shader_source << line_number++ << '\t';
            shader_source << line << '\n';
        }

        
        DLOG(ERROR) << shader_source.str();
        logErrors(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        throw ShaderProgramCreationError();
    }

    compiler_input.pop_back(); // Remove shader source so it isn't deleted.

    foreach (char const* s, compiler_input) {
        delete [] s; // Delete allocated strings.
    }
    return shader;
}

void ShaderProgram::makeProgram(GLuint program_id)
{
    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, m_defines, m_vertex_source);
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, m_defines, m_fragment_source);

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
    m_vertex_shader = vertex_shader;
    m_fragment_shader = fragment_shader;
}


void ShaderProgram::logErrors(GLuint object_id, PFNGLGETSHADERIVPROC shader_iv, PFNGLGETSHADERINFOLOGPROC shader_infolog)
{
    GLint log_length;

    shader_iv(object_id, GL_INFO_LOG_LENGTH, &log_length);
    char log[log_length];
    shader_infolog(object_id, log_length, NULL, log);
    DLOG(ERROR) << '\n' << log;
}
