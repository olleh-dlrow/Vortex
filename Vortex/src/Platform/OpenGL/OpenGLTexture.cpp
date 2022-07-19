﻿#include "vtpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Vortex/Core/Application.h"

#include <stb_image.h>

namespace Vortex
{
    void ParseFormat(const char* format, uint32_t& dataFormat, uint32_t& internalFormat,
                    uint32_t& pixelType)
    {
        // GL_[components​][size​][type​]
        if (strcmp(format, "RGB8") == 0)
        {
            dataFormat = GL_RGB;
            internalFormat = GL_RGB8;
            pixelType = GL_UNSIGNED_BYTE;
        }
        else if (strcmp(format, "RGBA8") == 0)
        {
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA8;
            pixelType = GL_UNSIGNED_BYTE;
        }
        else if (strcmp(format, "SRGB8") == 0)
        {
            dataFormat = GL_RGB;
            internalFormat = GL_SRGB8;
            pixelType = GL_UNSIGNED_BYTE;
        }
        else if (strcmp(format, "SRGBA8") == 0)
        {
            dataFormat = GL_RGBA;
            internalFormat = GL_SRGB8_ALPHA8;
            pixelType = GL_UNSIGNED_BYTE;
        }
        else if (strcmp(format, "RGB16") == 0)
        {
            dataFormat = GL_RGB;
            internalFormat = GL_RGB16;
            pixelType = GL_UNSIGNED_SHORT;
        }
        else if (strcmp(format, "RGBA16") == 0)
        {
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA16;
            pixelType = GL_UNSIGNED_SHORT;
        }
        else if (strcmp(format, "RGB16F") == 0)
        {
            dataFormat = GL_RGB;
            internalFormat = GL_RGB16F;
            pixelType = GL_FLOAT;
        }
        else if (strcmp(format, "RGBA16F") == 0)
        {
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA16F;
            pixelType = GL_FLOAT;
        }
        else
        {
            VT_CORE_ASSERT(0, "Unkown texture format");
        }
    }

    // difference of glTextureStorage* and glTexImage*
    // https://community.khronos.org/t/framebuffer-texture-glteximage2d-vs-gltexstorage2d/74839/2
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, bool MSAAOpened,
                                     const char* format)
        : m_Width(width), m_Height(height)
    {
        uint32_t pixelType;
        ParseFormat(format, m_DataFormat, m_InternalFormat, pixelType);
        
        if (!MSAAOpened)
        {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, pixelType, NULL);

            // if format has 16f, don't use this
            //glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            //glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            // TEST MSAA
            glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_RendererID);
            int nSamples = Application::Get().GetWindow().GetGraphicsContext().GetMSAANSamples();
            glTextureStorage2DMultisample(m_RendererID, nSamples, m_InternalFormat, width, height, GL_TRUE);
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool gammaCorrection)
        : m_Path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        VT_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        // set blend format
        GLenum internalFormat, dataFormat, pixelType = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            if (gammaCorrection)
            {
                internalFormat = GL_SRGB8_ALPHA8;
            }
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            if (gammaCorrection)
            {
                internalFormat = GL_SRGB8;
            }
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        VT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        //glGenTextures(1, &m_RendererID);
        //glBindTexture(GL_TEXTURE_2D, m_RendererID);
        //glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, pixelType, NULL);

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        //glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // tips: set texture data and its level in minmap(here is 0)
        //glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        VT_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }
    void OpenGLTexture2D::Unbind(uint32_t slot) const
    {
        glBindTextureUnit(slot, 0);
    }
}
