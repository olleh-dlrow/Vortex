#pragma once

#include <string>
#include <unordered_map>

#include "Vortex/Core/Core.h"
#include <glm/glm.hpp>
#include <optional>

namespace Vortex
{
    struct TextureMeta;
    class Texture;

    using TexPair = Pair<TextureMeta, Ref<Texture> >;

    enum class TextureType
    {
        NONE,
        TEX2D,
        TEX2D_MULTISAMPLE,
        CUBEMAP,
        TEX3D,
        ENUM_MAX
    };

    enum class TextureWrapMode
    {
        NONE,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        REPEAT,
        MIRRORED_REPEAT,
        ENUM_MAX
    };

    enum class TextureWrapAxis
    {
        NONE,
        S,
        T,
        R,
        ENUM_MAX
    };

    enum class TextureFilterMode
    {
        NONE,
        NEAREST,
        LINEAR,
        LINEAR_MIPMAP_LINEAR,
        ENUM_MAX
    };

    enum class TextureFilterOperation
    {
        NONE,
        MINIFY,
        MAGNIFY,
        ENUM_MAX
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetID() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind(uint32_t slot = 0) const = 0;

        virtual bool operator==(const Texture& other) const = 0;

        virtual TextureType GetType() const = 0;
        virtual std::string GetTextureFormat() const { return m_TextureFormat; }
        
        void SetMipmap(bool enable) { m_MipmapEnabled = enable; }
        bool GetMipmap() const { return m_MipmapEnabled; }

        TextureWrapMode GetWrapMode(TextureWrapAxis axis) const 
        { 
            return m_WrapModes.count(axis) > 0 ? m_WrapModes.at(axis) : TextureWrapMode::NONE;
        }
        void SetTextureWrapMode(TextureWrapAxis axis, TextureWrapMode mode) { m_WrapModes[axis] = mode; }

        TextureFilterMode GetFilterMode(TextureFilterOperation op) const 
        {
            return m_Filters.count(op) > 0 ? m_Filters.at(op) : TextureFilterMode::NONE;
        }
        void SetTextureFilterMode(TextureFilterOperation op, TextureFilterMode mode) { m_Filters[op] = mode; }

        void SetBorderColor(const glm::vec4& col) { m_BorderColor = col; }

        virtual void ApplySettings() const = 0;

        void RenderConfigGUI();

    protected:
        std::string                                                         m_TextureFormat;
        bool                                                                m_MipmapEnabled = false;
        HashMap<TextureWrapAxis, TextureWrapMode>                           m_WrapModes;
        HashMap<TextureFilterOperation, TextureFilterMode>                  m_Filters;
        std::optional<glm::vec4>                                            m_BorderColor;
        bool                                                                m_IsDirty = false;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(uint32_t width, uint32_t height, 
                                    const char* format = "RGBA8");
        // https://learnopengl-cn.github.io/05%20Advanced%20Lighting/02%20Gamma%20Correction/
        // when gammaCorrection setted, the color space will automatically changed from
        // gamma space to linear space
        static Ref<Texture2D> Create(const std::string& path, 
                                     bool gammaCorrection = false);

        virtual TextureType GetType() const override { return TextureType::TEX2D; }
    };

    class MultisampleTexture2D : public Texture2D
    {
    public:
        static Ref<MultisampleTexture2D> Create(uint32_t width, uint32_t height, int nSamples, 
                                                const char* format="RGBA8");

        virtual TextureType GetType() const override { return TextureType::TEX2D_MULTISAMPLE; }
    };

    class Cubemap : public Texture
    {
    public:
        virtual TextureType GetType() const override { return TextureType::CUBEMAP; }

        static Ref<Cubemap> Create(const std::vector<std::string>& facesPath);
        static Ref<Cubemap> Create(uint32_t width, uint32_t height, const char* format = "RGBA8");
    };

    // store information about some texture
    struct TextureMeta
    {
        // implement copy on write, false means reference to other's memory
        // the loaded texture is put in library, and not auto unload
        // the copyed texture is possessed by its creator, will auto destroy 
        // when its creator destroyed
        bool isCopyed;
    };

    // TODO:
    // not consider load of SRGB texture and cubemap
    class TextureLibrary
    {
    public:
        Ref<Texture> Load(const std::string& filepath);
        Ref<Texture> Get(const std::string& filepath);
        bool Exists(const std::string& filepath);

    public:
        static TextureLibrary& Instance();

    private:
        // key: path, value: pair of texture meta and texture ptr
        HashMap<std::string, TexPair > m_Textures;
    };
}
