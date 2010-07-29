/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include <map>
#include <set>

#include "shaderprogram.h"

using namespace std;

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

/**
 * @brief Manages the compilation of shaders from an übershader.
 */
class ShaderFactory
{
public:
    /**
     * The shader sources are read in the constructor.
     */
    ShaderFactory();

    /** Creates a ShaderProgram made from an übershader.
     *
     * @param defines Set of defines used in the compilation.
     *
     * @return The compiled ShaderProgram.
     */
    ShaderProgram& makeShader(set< string > defines);

    /**
     * Destructor.
     */
    ~ShaderFactory();

private:
    map< string, vector<GLuint> >* m_define_table;
    map< GLuint, ShaderProgram >*  m_program_table;

    char* m_vertex_source, *m_geometry_source, *m_fragment_source;
};




#endif // SHADERFACTORY_H
