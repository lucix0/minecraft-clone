#include <engine/material.h>

Material::Material(const std::string& texture_name, const std::string& program_name)
    : program(program_name), texture(texture_name) {  }

void Material::bind() {
    bgfx::setTexture(0, program.sampler(), texture.handle(), BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT);
    bgfx::setState(render_state);
}