#include "vtpch.h"
#include "Vortex/Renderer/Texture.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include <magic_enum.hpp>
#include <imgui.h>

namespace Vortex
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height,
                                    const char* format)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  
            return CreateRef<OpenGLTexture2D>(width, height, format);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, bool gammaCorrection)
    {
        switch (RendererAPI::GetAPI())
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

    Ref<Cubemap> Cubemap::Create(const std::vector<std::string>& facesPath)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLCubemap>(facesPath);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Cubemap> Cubemap::Create(uint32_t width, uint32_t height, const char* format)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLCubemap>(width, height, format);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<MultisampleTexture2D> MultisampleTexture2D::Create(uint32_t width, uint32_t height, int nSamples, const char* format)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLMultisampleTexture2D>(width, height, nSamples, format);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // The reason of PushID:
    // https://blog.csdn.net/weixin_40787463/article/details/106928337
    void Texture::RenderConfigGUI()
    {
        ImGui::PushID(GetID());
        ImGui::Text("Texture Settings:");
        for (auto& wrap : m_WrapModes)
        {
            auto label = magic_enum::enum_name(wrap.first);
            auto preVal = magic_enum::enum_name(wrap.second);
            int currentIdx = (int)wrap.second;
            
            if (ImGui::BeginCombo(label.data(),preVal.data()))
            {
                for (int i = (int)TextureWrapMode::NONE + 1; i < (int)TextureWrapMode::ENUM_MAX; i++)
                {
                    bool isSelected = (currentIdx == i);
                    auto val = magic_enum::enum_name(TextureWrapMode(i));
                    
                    if (ImGui::Selectable(val.data(), isSelected))
                    {
                        currentIdx = i;
                        SetTextureWrapMode(wrap.first, TextureWrapMode(i));
                        m_IsDirty = true;
                    }
                }
                ImGui::EndCombo();
            }
        }
        for (auto& filter : m_Filters)
        {
            auto label = magic_enum::enum_name(filter.first);
            auto preVal = magic_enum::enum_name(filter.second);
            int currentIdx = (int)filter.second;
            if (ImGui::BeginCombo(label.data(), preVal.data()))
            {
                for (int i = (int)TextureFilterMode::NONE + 1; i < (int)TextureFilterMode::ENUM_MAX; i++)
                {
                    bool isSelected = (currentIdx == i);
                    auto val = magic_enum::enum_name(TextureFilterMode(i));

                    if (ImGui::Selectable(val.data(), isSelected))
                    {
                        currentIdx = i;
                        SetTextureFilterMode(filter.first, TextureFilterMode(i));
                        m_IsDirty = true;
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (ImGui::Checkbox("Mipmap", &m_MipmapEnabled))
        {
            SetMipmap(m_MipmapEnabled);
            m_IsDirty = true;
        }

        if (m_IsDirty && ImGui::Button("Apply"))
        {
            ApplySettings();
            m_IsDirty = false;
        }
        ImGui::PopID();
    }
}
