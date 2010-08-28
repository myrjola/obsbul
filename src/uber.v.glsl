#version 330

#ifdef POSITION
// Attribute indexes automatically defined in EntityFactory.
layout (location = POSITION) in vec4 in_position;
layout (location = NORMAL) in vec4 in_normal;
layout (location = TEXCOORD) in vec2 in_texcoord;
layout (location = MATERIAL_IDX) in unsigned int in_material_idx;
#endif // POSITION

mat4 view_frustum(
    float angle_of_view,
    float aspect_ratio,
    float z_near,
    float z_far
) {
    return mat4(
        vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
        vec4(0.0, aspect_ratio/tan(angle_of_view),  0.0, 0.0),
        vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), 1.0),
        vec4(0.0, 0.0, -2.0*z_far*z_near/(z_far-z_near), 0.0)
    );
}

mat4 scale(float factor)
{
    return mat4(
        vec4(factor, 0.0, 0.0, 0.0),
        vec4(0.0, factor, 0.0, 0.0),
        vec4(0.0, 0.0, factor, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 rotate_x(float theta)
{
    return mat4(
        vec4(1.0,         0.0,         0.0, 0.0),
        vec4(0.0,  cos(theta),  sin(theta), 0.0),
        vec4(0.0, -sin(theta),  cos(theta), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

mat4 rotate_y(float theta)
{
    return mat4(
        vec4(cos(theta), 0.0, -sin(theta), 0.0),
        vec4(0.0,        1.0,  sin(theta), 0.0),
        vec4(sin(theta), 0.0,  cos(theta), 0.0),
        vec4(0.0,        0.0,         0.0, 1.0)
    );
}

mat4 translate(float x, float y, float z)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

out vec4 frag_diffuse;
out vec4 frag_specular;
out float frag_shininess;
out vec3 frag_worldview_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;

#ifdef MATERIALS

struct Material {
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

layout(std140) uniform materials {
    Material Materials[MATERIALS];
};

#endif // MATERIALS


void main(void)
{
    mat4 rotation = mat4(1.0);
//     mat4 rotation = rotate_x(radians(20.0)) * rotate_y(radians(70.0));
    #ifdef FRUSTUM
    gl_Position = view_frustum(radians(45.0), 1, 1.0, 10.0)
                * translate(0.0, 0.0, 5.5)
                * rotation
                * in_position;
    #endif // FRUSTUM
    
    #ifdef ORTHO
    gl_Position = in_position;
    #endif ORTHO
//     frag_normal = in_normal.xyz;
    frag_normal = (rotation * in_normal).xyz;
    frag_texcoord = in_texcoord;
    frag_worldview_pos = (translate(0.0, 0.0, 5.5) * in_position).xyz;
    frag_diffuse = vec4(0.8, 0.0, 0.0, 1.0);
    #ifdef MATERIALS
    frag_diffuse = Materials[in_material_idx].diffuse;
    frag_specular = Materials[in_material_idx].specular;
    frag_shininess = Materials[in_material_idx].shininess;
    #endif // MATERIALS
}

