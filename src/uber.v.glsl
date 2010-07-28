#version 330

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texcoord;
in vec3 in_tangent;
in vec3 in_bitangent;

out vec3 frag_normal;
out vec3 frag_vertex_to_light;
out vec3 frag_half_vector;
out vec2 frag_texcoord;
out vec3 frag_tangent;
out vec3 frag_bitangent;

uniform vec3 viewer_position;
uniform vec3 lightsources[];
uniform mat4 mvpmatrix;
uniform mat4 modelmatrix;
uniform float aspect_ratio;


mat4 translate(float x, float y, float z)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
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
        vec4(       0.0, 1.0,         0.0, 0.0),
        vec4(sin(theta), 0.0,  cos(theta), 0.0),
        vec4(       0.0, 0.0,         0.0, 1.0)
    );
}

mat4 scale(float x, float y, float z)
{
    return mat4(
        vec4(x,   0.0, 0.0, 0.0),
        vec4(0.0, y,   0.0, 0.0),
        vec4(0.0, 0.0, z,   0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat3 normalmat(mat4 mvpmat)
{
    return transpose(transpose(mat3(mvpmat[0].xyz,
                      mvpmat[1].xyz,
                      mvpmat[2].xyz)));
}


void main(void) {
//     vec3 lightpos = vec3(0.0, 20.0, 0.0);
    gl_Position = mvpmatrix * scale(aspect_ratio, 1.0, 1.0) * vec4(in_position, 1.0);
    vec4 worldspace_position =  modelmatrix * vec4(in_position, 1.0);
    vec3 vertex_to_viewer = viewer_position - worldspace_position.xyz;
    frag_vertex_to_light = lightsources[0] - worldspace_position.xyz;
//     frag_vertex_to_light = lightpos - worldspace_position.xyz;
//     frag_vertex_to_light = vec3(-1.0, 2.0, -2.0);
    frag_half_vector = vertex_to_viewer + frag_vertex_to_light;
    frag_texcoord = in_texcoord;
    frag_normal = (modelmatrix * vec4(in_normal, 0.0)).xyz;
//     frag_normal = in_normal;
//     frag_tangent = (modelmatrix * vec4(in_tangent, 0.0)).xyz;
    frag_tangent = in_tangent;
//     frag_bitangent = (modelmatrix * vec4(in_bitangent, 0.0)).xyz;
    frag_bitangent = in_bitangent;
}
