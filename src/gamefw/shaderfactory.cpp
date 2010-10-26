#include "../common.h"

#include "shaderfactory.h"

#include "gamefw.h"

#define VERTEX_PATH "src/uber.v.glsl"
#define GEOMETRY_PATH "src/uber.g.glsl"
#define FRAGMENT_PATH "src/uber.f.glsl"

using namespace gamefw;

ShaderFactory::ShaderFactory()
{
    m_program_table = new map< GLuint, shared_ptr<ShaderProgram> >();
    m_define_table = new map< string, vector<GLuint>* >();
    loadSources();
    DLOG(INFO) << "ShaderFactory created";
}

ShaderFactory::~ShaderFactory()
{
    pair<GLuint, shared_ptr<ShaderProgram>> program_table_pair;
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
    FileService& fileservice = Locator::getFileService();
    m_vertex_source = fileservice.fileToBuffer(VERTEX_PATH);
    m_geometry_source = fileservice.fileToBuffer(GEOMETRY_PATH);
    m_fragment_source = fileservice.fileToBuffer(FRAGMENT_PATH);
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

shared_ptr<ShaderProgram> ShaderFactory::makeShader(set< string > defines)
{
    vector<GLuint> possible_programs;

    if (defines.empty()) { // Defines not allowed to be empty.
        DLOG(ERROR) << "ShaderFactory::makeShader passed an empty defines set.";
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
