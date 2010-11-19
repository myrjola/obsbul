
#include "renderjob.h"
#include "shaderprogram.h"

using namespace gamefw;

RenderJob::RenderJob()
:
m_num_textures(0)
{
    m_buffer_objects.element_buffer = 0;
    m_buffer_objects.vao = 0;
    m_buffer_objects.vertex_buffer = 0;
    m_buffer_objects.vertex_extra_buffer = 0;
    m_uniforms.materials = 0;
}

RenderJob::~RenderJob()
{
    if (m_num_textures > 0) {
//         glDeleteTextures(m_num_textures, m_textures);
        delete [] m_textures;
    }
    glDeleteVertexArrays(1, &m_buffer_objects.vao);
    glDeleteBuffers(1, &m_buffer_objects.element_buffer);
    glDeleteBuffers(1, &m_buffer_objects.vertex_buffer);
    glDeleteBuffers(1, &m_buffer_objects.vertex_extra_buffer);
    glDeleteBuffers(1, &m_uniforms.materials);
}


GLuint RenderJob::getShaderProgramID()
{
    return m_shaderprogram->getProgramID();
}

void RenderJob::setShaderProgram(shared_ptr< ShaderProgram > shaderprogram)
{
    m_shaderprogram = shaderprogram;
}

    



