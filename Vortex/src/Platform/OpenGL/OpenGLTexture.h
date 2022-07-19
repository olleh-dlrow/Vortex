#pragma once

#include "Vortex/Renderer/Texture.h"

#include <glad/glad.h>

namespace Vortex
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        // https ://www.khronos.org/opengl/wiki/Image_Format
        OpenGLTexture2D(uint32_t width, uint32_t height, bool MSAAOpened,
                        const char* format);
        OpenGLTexture2D(const std::string& path, bool gammaCorrection);
        virtual ~OpenGLTexture2D();

        inline virtual uint32_t GetWidth() const override { return m_Width; }
        inline virtual uint32_t GetHeight() const override { return m_Height; }
        inline virtual uint32_t GetID() const override { return m_RendererID; }

        // set the pixel data of texture
        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }

    private:
        std::string         m_Path;
        uint32_t            m_Width, m_Height;
        uint32_t            m_RendererID;
        GLenum              m_InternalFormat;
        GLenum              m_DataFormat;
    };

}
