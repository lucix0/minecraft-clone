#include <include/shader_program.h>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& name) {
    std::string vertex_path = "shaders/bin/vs_" + name + ".bin";
    std::string frag_path = "shaders/bin/fs_" + name + ".bin";

    // Vertex Shader
    std::ifstream vertex_file(vertex_path, std::ios::binary | std::ios::ate);
    if (!vertex_file.is_open()) {
        std::cerr << "Failed to open shader file: " << vertex_path << "\n";
        return;
    }

    std::streamsize vsize = vertex_file.tellg();
    vertex_file.seekg(0, std::ios::beg);

    const bgfx::Memory* vertex_mem = bgfx::alloc(static_cast<uint32_t>(vsize) + 1);
    if (!vertex_file.read(reinterpret_cast<char*>(vertex_mem->data), vsize)) {
        std::cerr << "Failed to read shader file: " << vertex_path << "\n";
        return;
    }
    vertex_mem->data[vsize] = '\0'; 

    auto vertex_shader = bgfx::createShader(vertex_mem);

    // Fragment Shader
    std::ifstream frag_file(frag_path, std::ios::binary | std::ios::ate);
    if (!frag_file.is_open()) {
        std::cerr << "Failed to open shader file: " << frag_path << "\n";
        return;
    }

    std::streamsize fsize = frag_file.tellg();
    frag_file.seekg(0, std::ios::beg);

    const bgfx::Memory* frag_mem = bgfx::alloc(static_cast<uint32_t>(fsize) + 1);
    if (!frag_file.read(reinterpret_cast<char*>(frag_mem->data), fsize)) {
        std::cerr << "Failed to read shader file: " << frag_path << "\n";
        return;
    }
    frag_mem->data[fsize] = '\0'; 

    auto frag_shader = bgfx::createShader(frag_mem);

    m_program = bgfx::createProgram(vertex_shader, frag_shader, true);
    m_sampler = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
}

ShaderProgram::~ShaderProgram() {
    bgfx::destroy(m_program);
    bgfx::destroy(m_sampler);
}