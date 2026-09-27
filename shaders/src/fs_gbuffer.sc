$input v_position, v_normal, v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texture, 0);

void main() {
    gl_FragData[0] = v_position;
    gl_FragData[1] = v_normal;
    gl_FragData[2] = texture2D(s_texture, v_texcoord0) * v_color0;
}