$input a_position, a_normal, a_color0, a_texcoord0
$output v_position, v_normal, v_color0, v_texcoord0

#include <bgfx_shader.sh>

vec4 faceNormal(float faceIndex) {
    float face = floor(faceIndex + 0.5);
    float axis = floor(face * 0.5);
    float dir = 1.0 - 2.0 * (face - 2.0 * axis);
    vec3 mask = 1.0 - step(0.5, abs(vec3_splat(axis) - vec3(0.0, 1.0, 2.0)));
    return vec4(mask * dir, 0.0);
}

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = mul(u_model[0], vec4(a_position, 1.0));
    v_normal = faceNormal(a_normal.x);
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
}