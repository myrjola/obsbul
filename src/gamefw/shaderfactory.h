#include "../common.h"

#include <map>
#include <set>

#include "shaderprogram.h"
#include "openglversion.h"

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
     * @param opengl_version Determines the GLSL version. Defaults to OGL_3_3.
     **/
    ShaderFactory(OpenGLVersion opengl_version = OGL_3_3);

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
    
    const OpenGLVersion m_opengl_version;

    char const* m_vertex_source, *m_geometry_source, *m_fragment_source;
    string m_vertex_path, m_geometry_path, m_fragment_path;
    void loadSources();
    void deallocateSources();
};

}

#endif // SHADERFACTORY_H
