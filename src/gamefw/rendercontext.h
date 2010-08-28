
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <queue>

#define GL3_PROTOTYPES
#include <GL3/gl3.h>
#include "entity.h"

namespace gamefw {

class RenderJob;

/**
 * @brief Practically where the rendering happens.
 */
class RenderContext
{
public:
    RenderContext();
    ~RenderContext();
    
    /**
     * @brief Adds entity to the rendering pipeline.
     *
     * @param entity ditto.
     */
    void addToRenderQueue(Entity& entity);

    /**
     * @brief Renders the scene consisting of everything in the render queue.
     */
    void render();
    
private:
    struct {
        GLuint gbuffers, pbuffer;
    } m_fbo;

    struct {
        GLuint gbuffer, pbuffer;
    } m_depth_stencil_buffers;

    std::queue<Entity*> m_render_queue;
    Entity gbuffer;
    Entity pbuffer;
    
    void initBuffers(GLuint width, GLuint height);
    void renderGBuffers();
    void renderEntity(Entity& entity);
    void texParametersForRenderTargets();
    bool checkFramebuffer();
    void createDepthStencilBuffer(GLuint* buffer, GLuint width, GLuint height);
};

}

#endif // RENDERCONTEXT_H
