#version 120

attribute vec4 in_position;
attribute vec4 in_normal;
attribute vec2 in_texcoord;

uniform mat4 model;
uniform mat4 normalmatrix;
uniform mat4 mvp;

varying vec4 frag_diffuse;
varying vec4 frag_specular;
varying float frag_shininess;
varying vec3 frag_worldspace_pos;
varying vec3 frag_normal;
varying vec2 frag_texcoord;

void main(void)
{
    #ifdef FRUSTUM
    gl_Position = mvp
                * in_position;
    #endif // FRUSTUM

    #ifdef ORTHO
    gl_Position = in_position;
    #endif // ORTHO

    frag_diffuse = vec4(1.0);
    frag_normal = (normalmatrix * in_normal).xyz;
    frag_texcoord = in_texcoord;
    frag_worldspace_pos = (model * in_position).xyz;
}
