#version 120


varying vec4 frag_diffuse;
varying vec4 frag_specular;
varying float frag_shininess;
varying vec3 frag_normal;
varying vec3 frag_worldspace_pos;
varying vec2 frag_texcoord;

uniform float display_width, display_height;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

void main(void)
{
    vec4 diffuse;
    diffuse = frag_diffuse;
    #ifdef ALBEDO_TEX
    diffuse = texture2D(texture0, frag_texcoord);
    #endif // ALBEDO_TEX
    #ifndef ALBEDO_TEX
    diffuse = frag_diffuse;
    #endif // not ALBEDO_TEX
    float cos_theta = clamp(dot(normalize(frag_normal.xyz),
                                normalize(vec3(1.0, 1.0, 0.0))), 0.0, 1.0);
    diffuse = diffuse * (cos_theta + 0.1);
    gl_FragColor = diffuse;
}
