
/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/


#ifndef RENDERJOB_H
#define RENDERJOB_H

#include "gamefw.h"

/**
 * @brief Vertex representation.
 *
 * GPU:s like 128-bit (4 floats) aligned buffers.
 */
typedef struct {
    /// Vertex position.
    GLfloat position[4];
    /// Vertex surface normal.
    GLfloat normal[4];
    /// Vertex uv texture coordinate.
    GLfloat texcoord[2];
    /// Vertex material index.
    GLuint material_idx;
} t_vertex;

/**
 * @brief Extra buffers for the vertex representation.
 */
typedef struct {
    /// Vertex surface tangent.
    GLfloat tangent[4];
    /// Vertex surface bitangent.
    GLfloat bitangent[4];
} t_vertex_extra;

/**
 * @brief Mesh material properties.
 */
typedef struct {
    btVector3 diffuse;
    btVector3 specular;
    GLuint shininess;
} t_material;
    

namespace gamefw {

/**
 * @brief Mesh representation sent to rendering.
 */
class RenderJob
{
public:
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
        NORMAL_TEX
    } indices;

    
    struct {
        GLuint vao, vertex_buffer, vertex_extra_buffer, element_buffer;
    } m_buffer_objects;

    t_material* m_materials;
    GLuint num_materials;

    GLuint* m_textures;
    GLuint num_textures;

    struct {
        GLuint position;
        GLuint normal;
        GLuint texcoord;
        GLuint material_idx;
    } m_attributes;

    struct {
        GLuint materials;
        GLuint modelmatrix;
        GLuint normalmatrix;
        GLuint mvpmatrix;
    } m_uniforms;

    GLuint m_shaderprogram;
    
    btTransform m_modelmatrix;
    btTransform m_normalmatrix;
    btTransform m_mvpmatrix;

    int m_face_count, m_vertex_count;
};

}

#endif // RENDERJOB_H
