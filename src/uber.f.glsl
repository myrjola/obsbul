#version 330

in vec4 frag_diffuse;
in vec4 frag_specular;
in float frag_shininess;
in vec3 frag_normal;
in vec3 frag_worldview_pos;
in vec2 frag_texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

#ifdef GBUFFER
layout(location = 0) out vec4 out_color;

vec2 delta[8];
#define INIT_DELTA delta[0] = vec2(-1.0,1.0);delta[1] = vec2(1.0,-1.0);delta[2] = vec2(-1.0,1.0);delta[3] = vec2(1.0,1.0);delta[4] = vec2(-1.0,0.0);delta[5] = vec2(1.0,0.0);delta[6] = vec2(0.0,-1.0);delta[7] = vec2(0.0,1.0);

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

// Antialiasing using the edge detection factor.
vec4 antialias(vec2 pixel_size,
               float factor)
{
    INIT_DELTA

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for(int i = 0; i < 8; i++) {
        color += texture(texture0, // TODO: diffuse texture should be changed to almost final image
                         frag_texcoord + delta[i] * pixel_size * factor);
    }
    color += texture(texture0, frag_texcoord);
    color *= 1.0/9.0; // 8 + 1 terms.
    return color;
}

#endif // GBUFFER

#ifndef GBUFFER
layout(location = OUTG_DIFFUSE) out vec4 out_diffuse;
layout(location = OUTG_SPECULAR) out vec4 out_specular;
layout(location = OUTG_NORMAL) out vec4 out_normal;
layout(location = OUTG_POSITION) out vec4 out_position;
#endif // GBUFFER



void main(void)
{
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
    vec4 specular = texture(texture1, frag_texcoord);
    vec4 position = texture(texture3, frag_texcoord);
    vec3 to_light = vec3(5.0, 0.0, 0.0) - position.xyz;
    float cos_theta = dot(normalize(normal.xyz), normalize(to_light));
    vec2 pixel_size = vec2(1.0/800.0, 1.0/400.0);
    float factor = detect_edges(pixel_size, 0.8);
    out_color = 
//         vec4(factor, factor, factor, 1.0);
        antialias(pixel_size, factor) * cos_theta;
    #endif // GBUFFER
    
    #ifndef GBUFFER
    out_diffuse = diffuse;
    out_specular = frag_specular;
    out_normal = vec4(normalize(frag_normal), 1.0);
    out_position = vec4(frag_worldview_pos, 1.0);
    #endif // not GBUFFER
}