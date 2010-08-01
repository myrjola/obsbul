/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "shaderfactory.h"

#include "gamefw.h"

#define VERTEX_PATH "src/uber.v.glsl"
#define GEOMETRY_PATH "src/uber.g.glsl"
#define FRAGMENT_PATH "src/uber.f.glsl"

ShaderFactory::ShaderFactory()
{
    m_program_table = new map< GLuint, ShaderProgram* >();
    m_define_table = new map< string, vector<GLuint> >();
    FileService& fileservice = Locator::getFileService();
    m_vertex_source = fileservice.fileToBuffer(VERTEX_PATH);
    m_geometry_source = fileservice.fileToBuffer(GEOMETRY_PATH);
    m_fragment_source = fileservice.fileToBuffer(FRAGMENT_PATH);
    DLOG(INFO) << "ShaderFactory created";
}

ShaderFactory::~ShaderFactory()
{
    delete m_program_table;
    delete m_define_table;
    delete [] m_vertex_source;
    delete [] m_geometry_source;
    delete [] m_fragment_source;
}

ShaderProgram& ShaderFactory::makeShader(set< string > defines)
{
    vector<GLuint> possible_programs;

    map< string, vector<GLuint> >::iterator result;
    result = m_define_table->find(*defines.begin());
    if (result != m_define_table->end()) { // If first define found in map.
        possible_programs = result->second;
    }
    foreach (GLuint id, possible_programs) {
        ShaderProgram& candidate = *m_program_table->at(id);
        if (candidate.getDefines() == defines) { // If matching program.
            return candidate;
        }
    }
    // No matching programs found.

    // Create new shader program and assign it to the map.
    ShaderProgram* program =  new ShaderProgram(m_vertex_source,
            m_geometry_source,
            m_fragment_source,
            defines);
    GLuint id = program->getProgramID();
    foreach (string current_define, defines) {
        map<string, vector<GLuint> >::iterator result;
        if (m_define_table->find(current_define) == m_define_table->end()) {
            // Define not found.
            (*m_define_table)[current_define] = *new vector<GLuint>();
        }
        (*m_define_table)[current_define].push_back(id);
    }
    m_program_table->insert(make_pair(id, program));
    return *m_program_table->at(id);
}
