/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/


#ifndef RENDERJOB_H
#define RENDERJOB_H

#include "gamefw.h"

/**
 * @brief Mesh representation sent to rendering.
 */
class RenderJob
{
private:
    struct {
        GLuint vao, vbo[4], texture, normal_map;
        GLfloat *vertex_data, *normal_data, *tangent_data,
        *bitangent_data, *texcoord_data;
        GLuint* element_data;
    } m_gldata; // TODO: better name.

    struct {
        btVector3 ambient;
        btVector3 diffuse;
        btVector3 specular;
        GLfloat shininess;
    } m_material;
    btTransform m_modelmatrix;
    btTransform m_normalmatrix;
    btTransform m_mvpmatrix;

    int m_face_count, m_vertex_count;

};

#endif // RENDERJOB_H
