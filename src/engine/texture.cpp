#include <iostream>
#include <engine/texture.h>
#include <engine/stb_image.h>

Texture::Texture(const std::string& file_name) {
    int imageWidth, imageHeight, imageChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *img = stbi_load(std::string("textures/" + file_name).c_str(), &imageWidth, &imageHeight, &imageChannels, 4);
    if (img == nullptr) {
        std::cerr << "Failed to load image " << file_name << ":" << stbi_failure_reason() << std::endl;
    }

    if (img != nullptr) {
        m_handle = bgfx::createTexture2D(imageWidth, imageHeight, false, 1, bgfx::TextureFormat::RGBA8, 0, bgfx::copy(img, imageWidth * imageHeight * 4));
        stbi_image_free(img);
    }
}

Texture::~Texture() {
    if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
        m_handle = BGFX_INVALID_HANDLE;
    }
}