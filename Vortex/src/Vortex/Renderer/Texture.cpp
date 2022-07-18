#include "vtpch.h"
#include "Vortex/Renderer/Texture.h"

#include "Vortex/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vortex
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, bool MSAAOpened)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  
            return CreateRef<OpenGLTexture2D>(width, height, MSAAOpened);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
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
