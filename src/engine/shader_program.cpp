#include <engine/shader_program.h>
#include <fstream>
#include <iostream>

bgfx::ShaderHandle ShaderProgram::loadShader(const std::string& name, ShaderType type) {
    // Create shader file path depending on type of shader
    std::string path{};
    if (type == ShaderType::VERTEX)        path = "shaders/bin/vs_" + name + ".bin";
    else if (type == ShaderType::FRAGMENT) path = "shaders/bin/fs_" + name + ".bin";

    // Open shader binary file
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << "\n";
        return BGFX_INVALID_HANDLE;
    }

    // Load shader file into memory
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> vec(size + 1);
    vec[size] = '\0';
    if (!file.read(reinterpret_cast<char*>(vec.data()), size)) {
        std::cerr << "Failed to read shader file: " << path << "\n";
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory* mem = bgfx::copy(vec.data(), vec.size() * sizeof(uint8_t));

    return bgfx::createShader(mem);
}

ShaderProgram::ShaderProgram(const std::string& name) {
    auto vShader = loadShader(name, ShaderType::VERTEX);
    auto fShader = loadShader(name, ShaderType::FRAGMENT);
    if (!bgfx::isValid(vShader) || !bgfx::isValid(fShader)) {
        if (bgfx::isValid(vShader)) bgfx::destroy(vShader);
        if (bgfx::isValid(fShader)) bgfx::destroy(fShader);
    }

    if (bgfx::isValid(vShader) && bgfx::isValid(fShader)) {
        m_program = bgfx::createProgram(vShader, fShader, true);
        m_sampler = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
    }
}

ShaderProgram::~ShaderProgram() {
    if (bgfx::isValid(m_program)) {
        bgfx::destroy(m_program);
        m_program = BGFX_INVALID_HANDLE;
    }

    if (bgfx::isValid(m_sampler)) {
        bgfx::destroy(m_sampler);
        m_sampler = BGFX_INVALID_HANDLE;
    }
}