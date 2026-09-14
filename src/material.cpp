#include <include/material.h>

Material::Material(const std::string& texture_name, const std::string& program_name)
    : program(program_name), texture(texture_name) {  }

void Material::bind() {
    bgfx::setTexture(0, program.sampler(), texture.handle(), BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
}