#include "vtpch.h"
#include "Vortex/Renderer/Texture.h"

#include "Vortex/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vortex
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, bool MSAAOpened,
                                    const char* format)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  
            return CreateRef<OpenGLTexture2D>(width, height, MSAAOpened, format);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, bool gammaCorrection)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path, gammaCorrection);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    Ref<Texture> TextureLibrary::Load(const std::string& filepath)
    {
        Ref<Texture> tex = Texture2D::Create(filepath);
        m_Textures[filepath] = TexPair{TextureMeta{false}, tex};
        return tex;
    }
    Ref<Texture> TextureLibrary::Get(const std::string& filepath)
    {
        return m_Textures[filepath].second;
    }
    bool TextureLibrary::Exists(const std::string& filepath)
    {
        if (m_Textures.count(filepath) > 0)
        {
            return true;
        }
        return false;
    }
    TextureLibrary& TextureLibrary::Instance()
    {
        static TextureLibrary instance;
        return instance;
    }
}
