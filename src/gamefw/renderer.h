
#ifndef RENDERER_H
#define RENDERER_H

#include <queue>

#include "entity.h"

namespace gamefw {

class PointLight;


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
    Renderer(const GLuint display_width, const GLuint display_height);
    ~Renderer();
    
    /**
     * @brief Adds entity to the rendering pipeline.
     *
     * @param entity ditto.
     */
    void addToRenderQueue(const gamefw::Entity& entity);

    /**
     * @brief Adds pointlight to the rendering pipeline.
     *
     * @param pointlight ditto.
     * @return void
     **/
    void addToPointLightQueue(const gamefw::PointLight& pointlight);

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

    struct {
        GLuint pointlights, spotlights;
    } m_uniform_blocks;

    std::queue<const Entity*> m_render_queue;
    std::queue<const PointLight*> m_pointlight_queue;
    
    Entity m_gbuffer;
    Entity m_pbuffer;
    Entity m_ppbuffer;

    shared_ptr<Entity> m_camera;

    void initBuffers(const GLuint width, const GLuint height);
    void renderGBuffers();
    void renderEntity(const gamefw::Entity& entity);
    void texParametersForRenderTargets() const;
    bool checkFramebuffer() const;
    void createDepthStencilBuffer(GLuint* buffer, const GLuint width,
                                  const GLuint height);
    void createTexturesForFBO(shared_ptr<RenderJob> renderjob,
                              const GLuint num_textures,
                              const GLenum internalformats[],
                              const GLuint size_divisor[],
                              const GLenum types[]);
    void renderPBuffers();
    void renderPPBuffers();
    uint loadLightsIntoUniformBlocks();
};

}

#endif // RENDERER
