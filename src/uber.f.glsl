#version 330

in vec3 frag_normal;
in vec3 frag_vertex_to_light;
in vec3 frag_half_vector;
in vec2 frag_texcoord;
in vec3 frag_tangent;
in vec3 frag_bitangent;

out vec4 out_color;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform sampler2D texture;
uniform sampler2D normal_map;
uniform mat4 modelmatrix;



void main(void) {
    vec3 diffuse = texture2D(texture, frag_texcoord).xyz;
    vec3 normal_map_rbg = texture2D(normal_map, frag_texcoord).xyz;
    vec3 normal;
    normal.x = -2 * (normal_map_rbg.r - 0.5);
    normal.y = 2 * (normal_map_rbg.b - 0.5);
    normal.z = -normal_map_rbg.g;

    vec3 bitangent = normalize(frag_normal) * normalize(frag_tangent);

    mat3 tbnmatrix = (mat3(normalize(frag_tangent),
                          normalize(bitangent),
                          normalize(normal)));

    normal = normalize(tbnmatrix * normal);
//     normal = normalize(frag_normal);
//     normal = normalize(frag_bitangent) * normalize(frag_tangent);

//     vec3 vertex_to_light = tbnmatrix * normalize(frag_vertex_to_light);
//     vec3 half_vector     = tbnmatrix * normalize(frag_half_vector);

    float norm_dot_half = clamp(dot(normal, normalize(frag_half_vector)), 0.0, 1.0);
    float norm_dot_light = clamp(dot(normal, normalize(frag_vertex_to_light)), 0.0, 1.0);
    float pi = 3.14;
    vec3 Kd = diffuse / pi;
    vec3 Ks = specular * (shininess + 8) / (8 * pi);

    out_color = vec4((Kd + Ks * pow(norm_dot_half, shininess)) * norm_dot_light + 0.05 * diffuse, 1.0);
//     out_color = vec4(normalize(frag_normal), 1.0);

}
