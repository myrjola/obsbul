
#include "../common.h"
#include "rendercontext.h"

#include "renderjob.h"
#include "locator.h"
#include "fileservice.h"
#include "gamefw.h"

using namespace gamefw;

RenderContext::RenderContext()
{
    initBuffers(800, 600);
}

RenderContext::~RenderContext()
{
    glDeleteRenderbuffers(1, &m_depth_stencil_buffers.gbuffer);
    glDeleteRenderbuffers(1, &m_depth_stencil_buffers.pbuffer);
}

void RenderContext::createDepthStencilBuffer(GLuint* buffer, GLuint width, GLuint height)
{
    glGenRenderbuffers(1, buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil_buffers.gbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, m_depth_stencil_buffers.gbuffer);
}

void RenderContext::texParametersForRenderTargets()
{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
}

bool RenderContext::checkFramebuffer()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    string error = "Framebuffer object error: ";
    
    switch(status)
    {
    case GL_FRAMEBUFFER_UNDEFINED:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_UNDEFINED";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_UNSUPPORTED";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        DLOG(ERROR) << error << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
        break;
    }

    return status == GL_FRAMEBUFFER_COMPLETE;
}


void RenderContext::initBuffers(GLuint width, GLuint height)
{
    int num_textures = 8;

    // GBUFFER STAGE
    
    {
        gbuffer = Locator::getFileService().createEntity("gbuffer");
        glGenFramebuffers(1, &m_fbo.gbuffers);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.gbuffers);

        glActiveTexture(GL_TEXTURE0);
        shared_ptr<RenderJob> gbuffer_renderjob = gbuffer.getRenderJob();
        gbuffer_renderjob->m_textures = new GLuint[num_textures];
        gbuffer_renderjob->m_num_textures = num_textures;
        glGenTextures(num_textures, gbuffer_renderjob->m_textures);

        for (int i = 0; i < num_textures; i++) {
            glBindTexture(GL_TEXTURE_2D, gbuffer_renderjob->m_textures[i]);

            texParametersForRenderTargets();
            
            GLenum internalformat, type;
            switch (i) {
                case(renderjob_enums::OUTG_NORMAL):
                    internalformat = GL_RGB16F;
                    type = GL_FLOAT; break;

                case(renderjob_enums::OUTG_POSITION):
                    internalformat = GL_RGB32F;
                    type = GL_FLOAT; break;

                default:
                    internalformat = GL_RGB8;
                    type = GL_UNSIGNED_BYTE; break;
            }
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                internalformat,
                width,
                height,
                0,
                GL_RGB,
                type,
                0
            );

            GLint attachment = GL_COLOR_ATTACHMENT0 + i;
            
            glFramebufferTexture(GL_FRAMEBUFFER, attachment,
                                gbuffer_renderjob->m_textures[i], 0);
        }

        createDepthStencilBuffer(&m_depth_stencil_buffers.gbuffer, width, height);

        bool status = checkFramebuffer();
        assert(status);

        GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, draw_buffers);

        

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    // PBUFFER STAGE.

    {
        pbuffer = Locator::getFileService().createEntity("pbuffer");

        glGenFramebuffers(1, &m_fbo.pbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.pbuffer);

        glActiveTexture(GL_TEXTURE0);
        shared_ptr<RenderJob> pbuffer_renderjob = pbuffer.getRenderJob();
        pbuffer_renderjob->m_textures = new GLuint[num_textures];
        pbuffer_renderjob->m_num_textures = num_textures;
        glGenTextures(num_textures, pbuffer_renderjob->m_textures);

        for (int i = 0; i < num_textures; i++) {
            glBindTexture(GL_TEXTURE_2D, pbuffer_renderjob->m_textures[i]);
            
            texParametersForRenderTargets();

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB8,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                0
            );

            GLint attachment = GL_COLOR_ATTACHMENT0 + i;

            glFramebufferTexture(GL_FRAMEBUFFER, attachment,
                                pbuffer_renderjob->m_textures[i], 0);
        }

        createDepthStencilBuffer(&m_depth_stencil_buffers.pbuffer, width, height);

        bool status = checkFramebuffer();
        assert(status);

        GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, draw_buffers);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void RenderContext::render()
{
    renderGBuffers();
    
    renderPBuffers();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind main window's framebuffer.
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    renderEntity(pbuffer);
}

void RenderContext::addToRenderQueue(Entity& entity)
{
    m_render_queue.push(&entity);
}

void RenderContext::renderEntity(Entity& entity)
{
    shared_ptr<RenderJob> renderjob = entity.getRenderJob();
    GLuint program_id = renderjob->getShaderProgramID();
    glUseProgram(program_id);

    // Load textures.
    for (int i = 0; i < renderjob->m_num_textures; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderjob->m_textures[i]);
        string uniform_name("texture");
        uniform_name += (char) '0' + i;
        GLint location = glGetUniformLocation(program_id, uniform_name.c_str());
        glUniform1i(location, i);
    }

    // Bind material uniform block.
    if (renderjob->m_uniforms.materials != 0) {
        glBindBufferBase(GL_UNIFORM_BUFFER, renderjob_enums::MATERIAL,
                renderjob->m_uniforms.materials);
    }

    glBindVertexArray(renderjob->m_buffer_objects.vao);
    
    glEnableVertexAttribArray(renderjob_enums::POSITION);
    glEnableVertexAttribArray(renderjob_enums::NORMAL);
    glEnableVertexAttribArray(renderjob_enums::TEXCOORD);
    glEnableVertexAttribArray(renderjob_enums::MATERIAL_IDX);
    glDrawElements(GL_TRIANGLES, renderjob->m_vertex_count, GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(renderjob_enums::POSITION);
    glDisableVertexAttribArray(renderjob_enums::NORMAL);
    glDisableVertexAttribArray(renderjob_enums::TEXCOORD);
    glDisableVertexAttribArray(renderjob_enums::MATERIAL_IDX);

    // Cleanup.
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBufferBase(GL_UNIFORM_BUFFER, renderjob_enums::MATERIAL, 0);
}

void RenderContext::renderGBuffers()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo.gbuffers);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    while (!m_render_queue.empty()) {
        Entity* current_entity = m_render_queue.front();
        m_render_queue.pop();
        renderEntity(*current_entity);
    }
}

void RenderContext::renderPBuffers()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo.pbuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GLuint program_id = gbuffer.getRenderJob()->getShaderProgramID();
    glm::vec3 viewer_position = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(program_id, "viewer_position"),
            1, glm::value_ptr(viewer_position));
    renderEntity(gbuffer);

}


