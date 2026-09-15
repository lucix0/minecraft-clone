#pragma once
#include <bgfx/bgfx.h>
#include <string>

class Texture {
private:
    bgfx::TextureHandle m_handle = BGFX_INVALID_HANDLE;
public:
    Texture(const std::string& file_name);
    ~Texture();

    bgfx::TextureHandle handle() const { return m_handle; }
};