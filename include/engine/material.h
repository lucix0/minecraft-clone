#pragma once
#include <engine/shader_program.h>
#include <engine/texture.h>

struct Material {
    ShaderProgram program;
    Texture texture;

    uint64_t render_state;

    Material(const std::string& texture_name, const std::string& program_name);

    void bind();
};