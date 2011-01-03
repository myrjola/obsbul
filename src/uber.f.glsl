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

uniform float display_width, display_height;


#define INIT_DELTA vec2 delta[8];\
delta[0] = vec2(-1.0,1.0);\
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

#define POINTLIGHTS 10
uniform int num_pointlights;

struct PointLight {
    vec4 position;
    vec4 rgbcolor_and_intensity;
};

layout(std140) uniform pointlights {
    PointLight PointLights[POINTLIGHTS];
};

#endif // GBUFFER

#ifdef BLOOM
// Blur used with bloom.
vec3 blur(vec2 pixel_size)
{
    float pi = 3.14;
    float sigma = 10.0;
    vec3 color = vec3(0.0, 0.0, 0.0);
    int w = 7;
    for(int i = 0; i < w; i++) {
        #ifdef PBUFFER
        vec2 coord = vec2(i, 0.0);
        #endif // PBUFFER
        #ifdef POSTPROC
        vec2 coord = vec2(0.0, i);
        #endif // POSTPROC
        vec3 t_pos = texture(texture1, frag_texcoord + coord * pixel_size).rgb;
        vec3 t_neg = texture(texture1, frag_texcoord - coord * pixel_size).rgb;
        color += (t_pos + t_neg) *
            (exp(-(i*i)/(2*sigma*sigma)) - exp(-(w*w)/(2*sigma*sigma)));
    }
    return color;
}
#endif // BLOOM

#ifdef PBUFFER
layout(location = OUTP_DIFFUSE) out vec4 out_diffuse;
layout(location = OUTP_SPECULAR) out vec4 out_specular;

// Antialiasing using the edge detection factor.
vec3 antialias(vec2 pixel_size,
               float factor)
{
    INIT_DELTA

    vec3 color = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < 8; i++) {
        color += texture(texture0,
                         frag_texcoord + delta[i] * pixel_size * factor).rgb;
    }
    color += texture(texture0, frag_texcoord).rgb;
    color *= 1.0/9.0; // 8 + 1 terms.
    return color;
}

#endif // PBUFFER

#ifndef PBUFFER
layout(location = OUTG_DIFFUSE) out vec4 out_diffuse;
layout(location = OUTG_SPECULAR) out vec4 out_specular;
layout(location = OUTG_NORMAL) out vec4 out_normal;
layout(location = OUTG_POSITION) out vec4 out_position;
layout(location = OUTG_EXTRA) out vec4 out_extra;
layout(location = OUTP_EDGES) out vec4 out_edges;
layout(location = OUTP_BLOOM) out vec4 out_bloom;
#endif // GBUFFER

#ifdef POSTPROC
layout(location = 0) out vec4 out_color;
#endif // POSTPROC


void main(void)
{
    float shin_encoder = 100.0;
    vec2 pixel_size = vec2(1.0/display_width, 1.0/display_height);
    
    #ifdef GBUFFER
    {
        vec4 normal = texture(texture2, frag_texcoord);
        vec3 diffuse = texture(texture0, frag_texcoord).rgb;
        vec3 specular = texture(texture1, frag_texcoord).rgb;
        vec3 position = texture(texture3, frag_texcoord).xyz;
        vec3 extra = texture(texture4, frag_texcoord).xyz;
        float shininess = extra.g * shin_encoder;
        float is_lightsource = extra.r;
        float is_skybox = extra.b;

        vec3 to_viewer = viewer_position - position.xyz;

        vec3 diffuse_temp = vec3(0.0, 0.0, 0.0);
        vec3 specular_temp = vec3(0.0, 0.0, 0.0);

        for (int i = 0; i < num_pointlights; i++) {
            vec3 lightpos = PointLights[i].position.xyz;
            vec3 color = PointLights[i].rgbcolor_and_intensity.rgb;
            float intensity = PointLights[i].rgbcolor_and_intensity.a;
            vec3 to_light = lightpos - position.xyz;
            color *= intensity;
            float distance = length(to_light);
            color /= distance * distance;
            vec3 half_vector = to_viewer + to_light;
            float norm_dot_half = clamp(dot(normal.xyz, normalize(half_vector)), 0.0, 1.0);
            float cos_theta = clamp(dot(normalize(normal.xyz), normalize(to_light)), 0.0, 1.0);
            diffuse_temp += diffuse * color * cos_theta;
            specular_temp += max(specular * color * pow(norm_dot_half, is_lightsource * shininess), 0.0);
        }
        out_diffuse = vec4(diffuse_temp * is_lightsource, 1.0);
        if (is_skybox <= 0.1) {
            out_diffuse = vec4(diffuse, 1.0);
        }
        out_specular = vec4(specular_temp, 1.0);
        #ifdef ANTIALIAS
        float factor = detect_edges(pixel_size, 1.0);
        out_edges = vec4(factor, factor, factor, 1.0);
        #endif // ANTIALIAS
    }
    #endif // GBUFFER

    #ifdef PBUFFER
    {
        #ifdef BLOOM
        out_specular = vec4(blur(pixel_size), 1.0);
        #else
        out_specular = texture(texture1, frag_texcoord);
        #endif // BLOOM
        float factor = texture(texture2, frag_texcoord).r;
        out_diffuse =
            #ifdef ANTIALIAS
            vec4(antialias(pixel_size, factor), 1.0);
            #else
            texture(texture0, frag_texcoord);
            #endif // ANTIALIAS
    }
    #endif // PBUFFER

    #ifdef POSTPROC
    {
        vec3 diffuse = texture(texture0, frag_texcoord).rgb;
        #ifdef BLOOM
        vec3 specular = blur(pixel_size);
        #else
        vec3 specular = texture(texture1, frag_texcoord).rgb;
        #endif // BLOOM
        out_color = vec4(diffuse + specular, 1.0);
    }
    #endif // POSTPROC
    
    #ifndef GBUFFER
    #ifndef PBUFFER
    #ifndef POSTPROC
    {
        vec4 diffuse;
        float alpha = 1.0;
        #ifdef ALBEDO_TEX
        diffuse = texture(texture0, frag_texcoord);
        alpha = diffuse.a;
        #endif ALBEDO_TEX
        if (alpha > 0.1) {
            #ifndef ALBEDO_TEX
            diffuse = frag_diffuse;
            #endif // not ALBEDO_TEX
            out_diffuse = diffuse;
            out_specular = vec4(frag_specular);
            out_normal = vec4(normalize(frag_normal), 1.0);
            out_position = vec4(frag_worldspace_pos, 1.0);
            out_extra = vec4(1.0, frag_shininess / shin_encoder, 1.0, 1.0);

            #ifdef LIGHTSOURCE
            out_extra.r = 0.0;
            #endif // LIGHTSOURCE

            #ifdef SKYBOX
            out_extra.b = 0.0;
            #endif // SKYBOX
        }
    }
    #endif // not POSTPROC
    #endif // not PBUFFER
    #endif // not GBUFFER
}