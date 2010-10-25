#version 330

in vec4 frag_diffuse;
in vec4 frag_specular;
in float frag_shininess;
in vec3 frag_normal;
in vec3 frag_worldspace_pos;
in vec2 frag_texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

vec2 delta[8];
#define INIT_DELTA delta[0] = vec2(-1.0,1.0);\
delta[1] = vec2(1.0,-1.0);\
delta[2] = vec2(-1.0,1.0);\
delta[3] = vec2(1.0,1.0);\
delta[4] = vec2(-1.0,0.0);\
delta[5] = vec2(1.0,0.0);\
delta[6] = vec2(0.0,-1.0);\
delta[7] = vec2(0.0,1.0);

#ifdef GBUFFER
uniform vec3 viewer_position;

// Edge detection using normal map.
float detect_edges(
    vec2 pixel_size,
    float weight)
{
    INIT_DELTA
   
    vec4 normal = texture(texture2, frag_texcoord);
    float factor = 0.0;
    for(int i = 0; i < 4; i++) {
        vec4 t = texture(texture2, frag_texcoord + delta[i] * pixel_size);
        t -= normal;
        factor += dot(t, t);
    }
    factor = min(1.0,factor) * weight;
    return factor;
}
#endif //GBUFFER

#ifdef PBUFFER
layout(location = 0) out vec4 out_color;

// Antialiasing using the edge detection factor.
vec4 antialias(vec2 pixel_size,
               float factor)
{
    INIT_DELTA

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for(int i = 0; i < 8; i++) {
        color += texture(texture0,
                         frag_texcoord + delta[i] * pixel_size * factor);
    }
    color += texture(texture0, frag_texcoord);
    color *= 1.0/9.0; // 8 + 1 terms.
    return color;
}

#endif // PBUFFER

#ifndef PBUFFER
layout(location = OUTG_DIFFUSE) out vec4 out_diffuse;
layout(location = OUTG_SPECULAR) out vec4 out_specular;
layout(location = OUTG_NORMAL) out vec4 out_normal;
layout(location = OUTG_POSITION) out vec4 out_position;
layout(location = OUTP_EDGES) out vec4 out_edges;
layout(location = OUTP_BLOOM) out vec4 out_bloom;
#endif // GBUFFER



void main(void)
{
    float shin_encoder = 25.0;
    vec2 pixel_size = vec2(1.0/800.0, 1.0/400.0); // TODO: width and height uniforms
    vec4 diffuse;
    #ifdef ALBEDO_TEX
    diffuse = texture(texture0, frag_texcoord);
    #endif // ALBEDO_TEX
    #ifndef ALBEDO_TEX
    diffuse = frag_diffuse;
    #endif // not ALBEDO_TEX
    
    #ifdef GBUFFER
    vec4 normal = texture(texture2, frag_texcoord);
    diffuse = texture(texture0, frag_texcoord);
    vec4 spec_rgb_shininess = texture(texture1, frag_texcoord);
    vec4 specular = vec4(spec_rgb_shininess.rgb, 1.0);
    float shininess = spec_rgb_shininess.a * shin_encoder;
    vec4 position = texture(texture3, frag_texcoord);
    vec3 to_light = vec3(5.0, 0.0, 0.0) - position.xyz;
    vec3 to_viewer = viewer_position - position.xyz;
    vec3 half_vector = to_viewer + to_light;
    float norm_dot_half = clamp(dot(normal.xyz, normalize(half_vector)), 0.0, 1.0);
    float cos_theta = dot(normalize(normal.xyz), normalize(to_light));
    float factor = detect_edges(pixel_size, 0.5);
    // Kd and Ks from the book Real-time rendering by Akenine Möller et al.
    float pi = 3.14;
    vec3 Kd = diffuse.rgb / pi;
    vec3 Ks = specular.rgb * (shininess + 8) / (8 * pi);
//     out_diffuse = vec4(Kd * cos_theta, 1.0);
//     out_specular = vec4(Ks * pow(norm_dot_half, shininess), 1.0);
    out_diffuse = diffuse * cos_theta;
    out_specular = specular * pow(norm_dot_half, shininess);
    out_edges = vec4(factor, factor, factor, 1.0);
    #endif // GBUFFER

    #ifdef PBUFFER
    diffuse = texture(texture0, frag_texcoord);
    vec4 specular = texture(texture1, frag_texcoord);
    float factor = texture(texture2, frag_texcoord).r;
    out_color = vec4(antialias(pixel_size, factor).rgb + specular.rgb, 1.0);
    #endif // PBUFFER
    
    #ifndef GBUFFER
    #ifndef PBUFFER
    out_diffuse = diffuse;
    out_specular = vec4(frag_specular.rgb, frag_shininess / shin_encoder);
    out_normal = vec4(normalize(frag_normal), 1.0);
    out_position = vec4(frag_worldspace_pos, 1.0);
    #endif // not PBUFFER
    #endif // not GBUFFER
}