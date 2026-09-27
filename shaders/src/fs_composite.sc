$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_position, 0);
SAMPLER2D(s_normal, 1);
SAMPLER2D(s_albedo, 2);
SAMPLER2D(s_depth, 3);

void main() {
    vec4 position = texture2D(s_position, v_texcoord0);
    if (position.w < 0.5) {
        gl_FragColor = vec4(0.0, 0.616, 1.0, 1.0);
    } else {
        vec3 sunAngle = normalize(vec3(0.4, 1.0, 0.3));
        float diffuse = max(dot(texture2D(s_normal, v_texcoord0).xyz, sunAngle), 0.0) * 0.8 + 0.2;
        gl_FragColor = texture2D(s_albedo, v_texcoord0) * diffuse;
    }
}