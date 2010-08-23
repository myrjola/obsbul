
#include "renderjob.h"
#include "shaderprogram.h"

using namespace gamefw;

RenderJob::RenderJob() : m_buffer_objects{0, 0, 0, 0}
{}

RenderJob::~RenderJob()
{}


GLuint RenderJob::getShaderProgramID()
{
    return m_shaderprogram->getProgramID();
}

void RenderJob::setShaderProgram(shared_ptr< ShaderProgram > shaderprogram)
{
    m_shaderprogram = shaderprogram;
}

	



