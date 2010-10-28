
#ifndef RENDERER_H
#define RENDERER_H

#include <queue>

#define GL3_PROTOTYPES
#include <GL3/gl3.h>
#include "entity.h"

namespace gamefw {

class RenderJob;

/**
 * @brief Where the magic happens.
 */
class Renderer
{
public:
    /**
     * @brief Creates renderer for screen with given dimensions.
     *
     * @param display_width Width of display in pixels.
     * @param display_height Height of display in pixels.
     */
    Renderer(uint display_width, uint display_height);
    ~Renderer();
    
    /**
     * @brief Adds entity to the rendering pipeline.
     *
     * @param entity ditto.
     */
    void addToRenderQueue(Entity& entity);

    /**
     * @brief Change given Entity to active camera.
     * 
     * @param camera ditto.
     */
    void changeCamera(shared_ptr<Entity> camera);

    /**
     * @brief Renders the scene consisting of everything in the render queue.
     */
    void render();
    
private:
    float m_display_width, m_display_height, m_aspect_ratio;
    
    struct {
        GLuint gbuffer, pbuffer, ppbuffer;
    } m_fbo;

    struct {
        GLuint gbuffer, pbuffer;
    } m_depth_stencil_buffers;

    std::queue<Entity*> m_render_queue;
    Entity m_gbuffer;
    Entity m_pbuffer;
    Entity m_ppbuffer;

    shared_ptr<Entity> m_camera;

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
