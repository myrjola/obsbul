/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/


#ifndef RENDERJOB_H
#define RENDERJOB_H

#include "gamefw.h"

class btTransform;

class RenderJob
{
    public:
        btTransform modelmatrix;
        btTransform normalmatrix;
        btTransform mvpmatrix;
    
        int face_count, vertex_count;
        
    
    private:
        GLuint vao, vbo[4], texture, normal_map;
        GLfloat *vertex_data, *normal_data, *tangent_data,
                *bitangent_data, *texcoord_data;
        GLuint* element_data;
        
        struct {
            btVector3 ambient;
            btVector3 diffuse;
            btVector3 specular;
            GLfloat shininess;
        } material;
};

#endif // RENDERJOB_H
