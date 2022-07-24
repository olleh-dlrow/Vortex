#include "vtpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Renderer/GraphicsContext.h"

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
        else if (strcmp(format, "RG16F") == 0)
        {
            dataFormat = GL_RG;
            internalFormat = GL_RG16F;
            pixelType = GL_FLOAT;
        }
        else
        {
            VT_CORE_ASSERT(0, "Unkown texture format");
        }
    }


    GLint ParseTextureWrapMode(TextureWrapMode mode)
    {
        switch (mode)
        {
        case Vortex::TextureWrapMode::NONE:
            return GL_NONE;
        case Vortex::TextureWrapMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case Vortex::TextureWrapMode::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        case Vortex::TextureWrapMode::REPEAT:
            return GL_REPEAT;
        case Vortex::TextureWrapMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        default:
            return GL_NONE;
        }
    }

    GLenum ParseTextureWrapAxis(TextureWrapAxis axis)
    {
        switch (axis)
        {
        case Vortex::TextureWrapAxis::S:
            return GL_TEXTURE_WRAP_S;
        case Vortex::TextureWrapAxis::T:
            return GL_TEXTURE_WRAP_T;
        case Vortex::TextureWrapAxis::R:
            return GL_TEXTURE_WRAP_R;
        default:
            return GL_NONE;
        }
    }

    GLint ParseTextureFilterMode(TextureFilterMode mode)
    {
        switch (mode)
        {
        case Vortex::TextureFilterMode::NONE:
            return GL_NONE;
        case Vortex::TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case Vortex::TextureFilterMode::LINEAR:
            return GL_LINEAR;
        case Vortex::TextureFilterMode::LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return GL_NONE;
        }
    }

    GLenum ParseTextureFilterOperation(TextureFilterOperation op)
    {
        switch (op)
        {
        case Vortex::TextureFilterOperation::MINIFY:
            return GL_TEXTURE_MIN_FILTER;
        case Vortex::TextureFilterOperation::MAGNIFY:
            return GL_TEXTURE_MAG_FILTER;
        default:
            return GL_NONE;
        }
    }

    GLenum ParseTextureType(TextureType type)
    {
        switch (type)
        {
        case TextureType::TEX2D:
            return GL_TEXTURE_2D;
        case TextureType::TEX2D_MULTISAMPLE:
            return GL_TEXTURE_2D_MULTISAMPLE;
        case TextureType::CUBEMAP:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return GL_NONE;
        }
    }

    void OpenGLApplySettings(const Texture& tex, 
        const HashMap<TextureWrapAxis, TextureWrapMode>& wrapModes,
        const HashMap<TextureFilterOperation, TextureFilterMode>& filters,
        bool mipmapEnabled)
    {
        tex.Bind();
        for (auto& wrap : wrapModes)
        {
            glTexParameteri(ParseTextureType(tex.GetType()), ParseTextureWrapAxis(wrap.first), ParseTextureWrapMode(wrap.second));
        }
        for (auto& filter : filters)
        {
            glTexParameteri(ParseTextureType(tex.GetType()), ParseTextureFilterOperation(filter.first), ParseTextureFilterMode(filter.second));
        }
        if (mipmapEnabled)
        {
            glGenerateMipmap(ParseTextureType(tex.GetType()));
        }
    }

    // difference of glTextureStorage* and glTexImage*
    // https://community.khronos.org/t/framebuffer-texture-glteximage2d-vs-gltexstorage2d/74839/2
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, 
                                     const char* format)
        : m_Width(width), m_Height(height)
    {
        m_TextureFormat = format;
        uint32_t pixelType;
        ParseFormat(format, m_DataFormat, m_InternalFormat, pixelType);
        
       glGenTextures(1, &m_RendererID);
       glBindTexture(GL_TEXTURE_2D, m_RendererID);
       glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, pixelType, NULL);

       SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::LINEAR);
       SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::NEAREST);

       SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::REPEAT);
       SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::REPEAT);
       ApplySettings();
       /*glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

       glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); */
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool gammaCorrection)
        : m_Path(path)
    {
        bool hdr = stbi_is_hdr(path.c_str());
        if (!hdr)
        {
            CreateGeneral(path, gammaCorrection);
        }
        else
        {
            CreateHDR(path);
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::CreateGeneral(const std::string& path, bool gammaCorrection)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        VT_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        // set blend format
        GLenum internalFormat = 0, dataFormat = 0;
        // https://stackoverflow.com/questions/41396731/in-opengl-whats-the-difference-between-the-gl-red-and-gl-r-color-formats
        if (channels == 1)
        {
            internalFormat = GL_RED;
            m_TextureFormat = "RED";
            dataFormat = GL_RED;
        }
        else if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            m_TextureFormat = "RGBA8";
            if (gammaCorrection)
            {
                internalFormat = GL_SRGB8_ALPHA8;
                m_TextureFormat = "SRGBA8";
            }
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            m_TextureFormat = "RGB8";
            if (gammaCorrection)
            {
                internalFormat = GL_SRGB8;
                m_TextureFormat = "SRGB8";
            }
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        VT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        
        SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::LINEAR);
        SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::NEAREST);

        SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::REPEAT);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::REPEAT);
        ApplySettings();
        //glGenerateMipmap(GL_TEXTURE_2D);

        //glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // IBL TEST
        //glGenerateMipmap(GL_TEXTURE_2D);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    void Vortex::OpenGLTexture2D::CreateHDR(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
        VT_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;
        stbi_is_hdr(path.c_str());
       
        // set blend format
        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA16F;
            m_TextureFormat = "RGBA16F";
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB16F;
            m_TextureFormat = "RGB16F";
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        VT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_FLOAT, data);
        SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::LINEAR);
        SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::NEAREST);

        SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::CLAMP_TO_EDGE);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        ApplySettings();

        //glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        VT_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    void OpenGLTexture2D::Unbind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::ApplySettings() const
    {
        OpenGLApplySettings(*this, m_WrapModes, m_Filters, m_MipmapEnabled);
    }

    OpenGLCubemap::OpenGLCubemap(const std::vector<std::string>& facesPath)
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        int width = 0, height = 0, channels = 0;
        m_InternalFormat = 0;
        m_DataFormat = 0;
        for (unsigned int i = 0; i < facesPath.size(); i++)
        {
            unsigned char* data = stbi_load(facesPath[i].c_str(), &width, &height, &channels, 0);
            VT_CORE_ASSERT(data, "Cubemap texture failed to load at path: {}", facesPath[i].c_str());
            if (i == 0)
            {
                if (channels == 4)
                {
                    m_InternalFormat = GL_RGBA8;
                    m_TextureFormat = "RGBA8";
                    m_DataFormat = GL_RGBA;
                }
                else if (channels == 3)
                {
                    m_InternalFormat = GL_RGB8;
                    m_TextureFormat = "RGB8";
                    m_DataFormat = GL_RGB;
                }
                VT_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Unknown format");
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        m_Width = width; m_Height = height;

        SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::LINEAR);
        SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::LINEAR);

        SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::CLAMP_TO_EDGE);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        ApplySettings();

        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    OpenGLCubemap::OpenGLCubemap(uint32_t width, uint32_t height, const char* format)
        :m_Width(width), m_Height(height)
    {
        m_TextureFormat = format;
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        uint32_t pixelType = 0;
        ParseFormat(format, m_DataFormat, m_InternalFormat, pixelType);

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, width, height, 0, m_DataFormat, pixelType, nullptr);
        }

        SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::LINEAR);
        SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::LINEAR);

        SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::CLAMP_TO_EDGE);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        ApplySettings();

        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    OpenGLCubemap::~OpenGLCubemap()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLCubemap::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }
    void OpenGLCubemap::Unbind(uint32_t slot) const
    {
        // glBindTextureUnit(slot, 0);
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    void OpenGLCubemap::SetData(void* data, uint32_t size)
    {
        VT_CORE_ASSERT(0, "Not implement!");
    }

    void OpenGLCubemap::ApplySettings() const
    {
        OpenGLApplySettings(*this, m_WrapModes, m_Filters, m_MipmapEnabled);
    }


    OpenGLMultisampleTexture2D::OpenGLMultisampleTexture2D(uint32_t width, uint32_t height,
                                                  int nSamples , const char* format)
        :m_Width(width), m_Height(height)
    {
        m_TextureFormat = format;
        uint32_t pixelType;
        ParseFormat(format, m_DataFormat, m_InternalFormat, pixelType);

        // TEST MSAA
        //glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_RendererID);
        //glTextureStorage2DMultisample(m_RendererID, nSamples, m_InternalFormat, width, height, GL_TRUE);
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nSamples, m_InternalFormat, m_Width, m_Height, GL_TRUE);
        
        ApplySettings();
    }
    OpenGLMultisampleTexture2D::~OpenGLMultisampleTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }
    void OpenGLMultisampleTexture2D::SetData(void* data, uint32_t size)
    {
        VT_CORE_ASSERT(0, "Not implement!");
    }
    void OpenGLMultisampleTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
    }
    void OpenGLMultisampleTexture2D::Unbind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    void OpenGLMultisampleTexture2D::ApplySettings() const
    {
        OpenGLApplySettings(*this, m_WrapModes, m_Filters, m_MipmapEnabled);
    }
}
