#ifndef RENDERJOB_H
#define RENDERJOB_H

#include "gamefw.h"
   

namespace gamefw {

/**
 * @brief Mesh representation sent to rendering.
 */
class RenderJob
{
public:
    RenderJob();
    ~RenderJob();

    enum {
        // t_vertex
        POSITION,
        NORMAL,
        TEXCOORD,
        MATERIAL_IDX,
        // t_vertex_extra
        TANGENT = 0,
        BITANGENT,
        // textures
        ALBEDO_TEX = 0,
        NORMAL_TEX,
        // uniform block indices.
        MATERIAL = 0,
        GLOBAL,
        TRANSFORM
    } indices;

    
    struct {
        GLuint vao, vertex_buffer,
               vertex_extra_buffer, element_buffer;
    } m_buffer_objects;

    GLuint* m_textures;
    GLuint num_textures;

    struct {
        GLuint materials;
        GLuint transforms;
    } m_uniforms;

    GLuint m_shaderprogram;
    
    int m_vertex_count;
};

}

#endif // RENDERJOB_H
