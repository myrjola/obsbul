
#ifndef RENDERER_H
#define RENDERER_H

#include <queue>

#define GL3_PROTOTYPES
#include <GL3/gl3.h>
#include "entity.h"

namespace gamefw {

class RenderJob;

/**
 * @brief Practically where the rendering happens.
 */
class Renderer
{
public:
    Renderer();
    ~Renderer();
    
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
        GLuint gbuffer, pbuffer, ppbuffer;
    } m_fbo;

    struct {
        GLuint gbuffer, pbuffer;
    } m_depth_stencil_buffers;

    std::queue<Entity*> m_render_queue;
    Entity gbuffer;
    Entity pbuffer;
    Entity ppbuffer;

    float viewer_position;
    
    void initBuffers(GLuint width, GLuint height);
    void renderGBuffers();
    void renderEntity(Entity& entity);
    void texParametersForRenderTargets();
    bool checkFramebuffer();
    void createDepthStencilBuffer(GLuint* buffer, GLuint width, GLuint height);
    void renderPBuffers();
    void renderPPBuffers();
};

}

#endif // RENDERER
