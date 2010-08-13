#version 330

in vec4 frag_diffuse;
in vec3 frag_normal;
out vec3 frag_pos_to_viewer;

out vec4 out_color;

void main(void)
{
//     out_color = vec4(1.0, 0.0, 0.0, 1.0);
//     out_color = normalize(frag_normal);
//     out_color = frag_diffuse;
    vec3 to_light = vec3(0.7, 0.0, -1.0);
    float cos_theta = dot(normalize(frag_normal), normalize(to_light));
    out_color = frag_diffuse * cos_theta; //+ vec4(1.0, 1.0, 1.0, 1.0) * 0.1;
}