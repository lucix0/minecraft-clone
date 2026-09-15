#pragma once
#include <string>
#include <bgfx/bgfx.h>

class ShaderProgram {
private:
    bgfx::ProgramHandle m_program = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_sampler = BGFX_INVALID_HANDLE;
public:
    ShaderProgram(const std::string& name);
    ~ShaderProgram();

    bgfx::ProgramHandle handle() const { return m_program; }
    bgfx::UniformHandle sampler() const { return m_sampler; }
};