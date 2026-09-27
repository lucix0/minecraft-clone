#include "engine/deferred_renderer.h"

#include "bx/bx.h"

DeferredRenderer::DeferredRenderer()
    : m_compositeShader("composite") {
    // BGFX_TEXTURE_RT marks textures as render targets
    const uint64_t rtFlags = BGFX_TEXTURE_RT
        | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP   | BGFX_SAMPLER_V_CLAMP;

    // Create individual G-buffer textures
    m_gPosition = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::RGBA32F, rtFlags);
    m_gNormal = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::RGBA16F, rtFlags);
    m_gAlbedo = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::RGBA8, rtFlags);
    m_depth = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::D32F, rtFlags);

    m_gPositionSampler = bgfx::createUniform("s_position", bgfx::UniformType::Sampler);
    m_gNormalSampler = bgfx::createUniform("s_normal", bgfx::UniformType::Sampler);
    m_gAlbedoSampler = bgfx::createUniform("s_albedo", bgfx::UniformType::Sampler);
    m_depthSampler = bgfx::createUniform("s_depth", bgfx::UniformType::Sampler);

    bgfx::TextureHandle attachments[] = { m_gPosition, m_gNormal, m_gAlbedo, m_depth };
    m_gBuffer = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, true);

    bgfx::setViewFrameBuffer(m_geometryView, m_gBuffer);
    bgfx::setViewRect(m_geometryView, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setViewClear(m_geometryView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
    bgfx::setViewName(m_geometryView, "Geometry");

    bgfx::setViewFrameBuffer(m_compositeView, BGFX_INVALID_HANDLE);
    bgfx::setViewRect(m_compositeView, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setViewClear(m_compositeView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x009DFFFF, 1.0f, 0);
    bgfx::setViewName(m_compositeView, "Composite");

    m_viewTriLayout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

    float triangle[] = {
        -1, -1, 0, 0,  1,
         3, -1, 0, 2,  1,
        -1,  3, 0, 0, -1
    };

    auto mem = bgfx::copy(triangle, sizeof(triangle));
    m_viewTri = bgfx::createVertexBuffer(mem, m_viewTriLayout);
}

DeferredRenderer::~DeferredRenderer() {
    if (bgfx::isValid(m_gBuffer))
        bgfx::destroy(m_gBuffer);

    if (bgfx::isValid(m_viewTri))
        bgfx::destroy(m_viewTri);

    if (bgfx::isValid(m_gPositionSampler))
        bgfx::destroy(m_gPositionSampler);

    if (bgfx::isValid(m_gNormalSampler))
        bgfx::destroy(m_gNormalSampler);

    if (bgfx::isValid(m_gAlbedoSampler))
        bgfx::destroy(m_gAlbedoSampler);

    if (bgfx::isValid(m_depthSampler))
        bgfx::destroy(m_depthSampler);
}

void DeferredRenderer::render() {
    // Composite render
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
    bgfx::setVertexBuffer(0, m_viewTri);
    bgfx::setTexture(0, m_gPositionSampler, m_gPosition);
    bgfx::setTexture(1, m_gNormalSampler, m_gNormal);
    bgfx::setTexture(2, m_gAlbedoSampler, m_gAlbedo);
    bgfx::setTexture(3, m_depthSampler, m_depth);

    bgfx::submit(m_compositeView, m_compositeShader.handle());
}
