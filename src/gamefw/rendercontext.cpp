
#include "../common.h"
#include "rendercontext.h"

#include "entity.h"
#include "renderjob.h"

using namespace gamefw;

void RenderContext::addToRenderQueue(Entity& entity)
{
    m_render_queue.push(&entity);
}

void RenderContext::render()
{
    while (!m_render_queue.empty()) {
        Entity* current_entity = m_render_queue.front();
        m_render_queue.pop();
        shared_ptr<RenderJob> renderjob = current_entity->getRenderJob();
        
        glUseProgram(renderjob->m_shaderprogram->getProgramID());
        glBindVertexArray(renderjob->m_buffer_objects.vertex_buffer);

        glEnableVertexAttribArray(renderjob_enums::POSITION);
        glEnableVertexAttribArray(renderjob_enums::NORMAL);
        glEnableVertexAttribArray(renderjob_enums::TEXCOORD);
        glEnableVertexAttribArray(renderjob_enums::MATERIAL_IDX);
        glDrawElements(GL_TRIANGLES, renderjob->m_vertex_count, GL_UNSIGNED_SHORT, 0);
        glDisableVertexAttribArray(renderjob_enums::POSITION);
        glDisableVertexAttribArray(renderjob_enums::NORMAL);
        glDisableVertexAttribArray(renderjob_enums::TEXCOORD);
        glDisableVertexAttribArray(renderjob_enums::MATERIAL_IDX);

        
    }
}

