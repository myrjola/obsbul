#include "../common.h"

#include "shaderfactory.h"

#include "gamefw.h"

using namespace gamefw;

ShaderFactory::ShaderFactory()
{
    m_program_table = new map< GLuint, shared_ptr<ShaderProgram> >();
    m_define_table = new map< string, vector<GLuint>* >();
    
    if (GLEW_GET_VAR(__GLEW_VERSION_3_0)) {
        m_vertex_path = "src/uber.v.glsl";
        m_geometry_path = "src/uber.f.glsl";
        m_fragment_path = "src/uber.f.glsl";
    } else if (GLEW_GET_VAR(__GLEW_VERSION_2_1)) {
        m_vertex_path = "src/simple.v.glsl";
        m_geometry_path = "src/simple.g.glsl";
        m_fragment_path = "src/simple.f.glsl";
    } else {
        throw OpenGLError();
    }
    
    loadSources();
    
    LOG(logINFO) << "ShaderFactory created";
}

ShaderFactory::~ShaderFactory()
{
    pair<GLuint, shared_ptr<ShaderProgram> > program_table_pair;
    delete m_program_table;

    pair<string, vector<GLuint>* > define_table_pair;
    foreach (define_table_pair, *m_define_table) {
        delete define_table_pair.second;
    }
    delete m_define_table;
    deallocateSources();
}

void ShaderFactory::deallocateSources()
{
    delete[] m_vertex_source;
    delete[] m_geometry_source;
    delete[] m_fragment_source;
}

void ShaderFactory::loadSources()
{
    const FileService& fileservice = Locator::getFileService();
    m_vertex_source = fileservice.fileToBuffer(m_vertex_path);
    m_geometry_source = fileservice.fileToBuffer(m_geometry_path);
    m_fragment_source = fileservice.fileToBuffer(m_fragment_path);
}

void ShaderFactory::reloadShaders()
{
    deallocateSources();
    loadSources();

    typedef map< GLuint, shared_ptr<ShaderProgram> >  map_type;

    foreach(map_type::value_type& key_value_pair, *m_program_table) {
        key_value_pair.second->reloadProgram(m_vertex_source, m_geometry_source,
                                             m_fragment_source);
    }
}

shared_ptr<ShaderProgram> ShaderFactory::makeShader(const set< string >& defines)
{
    vector<GLuint> possible_programs;

    if (defines.empty()) { // Defines not allowed to be empty.
        LOG(logERROR) << "ShaderFactory::makeShader passed an empty defines set.";
        throw ShaderProgramCreationError();
    }

    map< string, vector<GLuint>* >::iterator result;
    result = m_define_table->find(*defines.begin());
    if (result != m_define_table->end()) { // If first define found in map.
        possible_programs = *result->second;
    }
    foreach (GLuint id, possible_programs) {
        shared_ptr<ShaderProgram> candidate =m_program_table->at(id);
        if (candidate->getDefines() == defines) { // If matching program.
            return candidate;
        }
    }
    // No matching programs found.

    // Create new shader program and assign it to the map.
    shared_ptr<ShaderProgram> program(new ShaderProgram(m_vertex_source,
                                      m_geometry_source,
                                      m_fragment_source,
                                      defines));
    GLuint id = program->getProgramID();
    foreach (string current_define, defines) {
        if (m_define_table->find(current_define) == m_define_table->end()) {
            // Define not found.
            (*m_define_table)[current_define] = new vector<GLuint>();
        }
        m_define_table->at(current_define)->push_back(id);
    }
    m_program_table->insert(make_pair(id, program));
    return program;
}
