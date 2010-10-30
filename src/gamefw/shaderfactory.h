#include "../common.h"

#include <map>
#include <set>

#include "shaderprogram.h"

using namespace std;

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

namespace gamefw {

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

    /**
     * Creates a ShaderProgram made from an übershader.
     *
     * @param defines Set of defines used in the compilation.
     *
     * @return The compiled ShaderProgram.
     */
    shared_ptr<ShaderProgram> makeShader(const std::set< string >& defines);

    /**
     * Reloads shader sources and updates the shader programs.
     */
    void reloadShaders();

    /**
     * Destructor.
     */
    ~ShaderFactory();

private:
    map< string, vector<GLuint>* >* m_define_table;
    map< GLuint, shared_ptr<ShaderProgram> >*  m_program_table;

    char const* m_vertex_source, *m_geometry_source, *m_fragment_source;
    void loadSources();
    void deallocateSources();
};

}

#endif // SHADERFACTORY_H
