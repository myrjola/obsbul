
#include "renderjob.h"
#include "shaderprogram.h"

using namespace gamefw;

RenderJob::RenderJob() : m_buffer_objects{0, 0, 0, 0}, m_num_textures(0)
{}

RenderJob::~RenderJob()
{
    if (m_num_textures > 0) {
        glDeleteTextures(m_num_textures, m_textures);
        delete m_textures;
    }
}


GLuint RenderJob::getShaderProgramID()
{
    return m_shaderprogram->getProgramID();
}

void RenderJob::setShaderProgram(shared_ptr< ShaderProgram > shaderprogram)
{
    m_shaderprogram = shaderprogram;
}

	



