/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "shaderfactory.h"

#include "locator.h"
#include "fileservice.h"

#define VERTEX_PATH "src/uber.v.glsl"
#define GEOMETRY_PATH "src/uber.g.glsl"
#define FRAGMENT_PATH "src/uber.f.glsl"

ShaderFactory::ShaderFactory() 
{
    program_table = new map< GLuint, ShaderProgram >();
    define_table = new map< string, vector<GLuint> >();
    FileService& fileservice = Locator::getFileService();
    vertex_source = fileservice.fileToBuffer(VERTEX_PATH);
    geometry_source = fileservice.fileToBuffer(GEOMETRY_PATH);
    fragment_source = fileservice.fileToBuffer(FRAGMENT_PATH);
    DLOG(INFO) << "ShaderFactory created";
}

ShaderFactory::~ShaderFactory()
{
    delete program_table;
    delete define_table;
    delete [] vertex_source;
    delete [] geometry_source;
    delete [] fragment_source;
}

ShaderProgram& ShaderFactory::makeShader(set< string > defines)
{
    vector<GLuint> possible_programs;

    map< string, vector<GLuint> >::iterator result;
    result = define_table->find(*defines.begin());
    if (result != define_table->end()) { // If first define found in map.
        possible_programs = result->second;
    }
    foreach (GLuint id, possible_programs) {
        ShaderProgram& candidate = program_table->at(id);
        if (candidate.getDefines() == defines) { // If matching program.
            return candidate;
        }
    }
    // No matching programs found.

    // Create new shader program and assign it to the map.
    ShaderProgram* program =  new ShaderProgram(vertex_source,
                                               geometry_source,
                                               fragment_source,
                                               defines);
    GLuint id = program->getProgramID();
    foreach (string current_define, defines) {
        map<string, vector<GLuint> >::iterator result;
        if (define_table->find(current_define) == define_table->end()) {
            // Define not found.
            (*define_table)[current_define] = *new vector<GLuint>();
        }
        (*define_table)[current_define].push_back(id);
    }
    program_table->insert(make_pair(id, *program));
    return program_table->at(id);
}
