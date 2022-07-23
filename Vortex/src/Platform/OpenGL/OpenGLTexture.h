#pragma once

#include "Vortex/Renderer/Texture.h"

#include <glad/glad.h>

namespace Vortex
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        // https ://www.khronos.org/opengl/wiki/Image_Format
        OpenGLTexture2D(uint32_t width, uint32_t height, 
                        const char* format);
        OpenGLTexture2D(const std::string& path, bool gammaCorrection);
        virtual ~OpenGLTexture2D();

        void CreateGeneral(const std::string& path, bool gammaCorrection);
        void CreateHDR(const std::string& path);

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

    class OpenGLMultisampleTexture2D : public MultisampleTexture2D
    {
    public:
        OpenGLMultisampleTexture2D(uint32_t width, uint32_t height, int nSamples, const char* format);
        virtual ~OpenGLMultisampleTexture2D();

        inline virtual uint32_t GetWidth() const override { return m_Width; }
        inline virtual uint32_t GetHeight() const override { return m_Height; }
        inline virtual uint32_t GetID() const override { return m_RendererID; }

        // set the pixel data of texture
        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLMultisampleTexture2D&)other).m_RendererID;
        }
    private:
        uint32_t            m_Width, m_Height;
        uint32_t            m_RendererID;
        GLenum              m_InternalFormat;
        GLenum              m_DataFormat;
    };

    class OpenGLCubemap : public Cubemap
    {
    public:
        OpenGLCubemap(const std::vector<std::string>& facesPath);
        OpenGLCubemap(uint32_t width, uint32_t height, const char* format);
        virtual ~OpenGLCubemap();

        virtual void GenerateMipmaps() const override;
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind(uint32_t slot = 0) const override;

        inline virtual uint32_t GetWidth() const override { return m_Width; }
        inline virtual uint32_t GetHeight() const override { return m_Height; }
        inline virtual uint32_t GetID() const override { return m_RendererID; }

        virtual void SetData(void* data, uint32_t size) override;

        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLCubemap&)other).m_RendererID;
        }

    private:
        uint32_t            m_RendererID;
        uint32_t            m_Width;
        uint32_t            m_Height;
        GLenum              m_InternalFormat;
        GLenum              m_DataFormat;
    };
}
