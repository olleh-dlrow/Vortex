#pragma once

#include <string>

#include "Vortex/Core/Core.h"

namespace Vortex
{
    struct TextureMeta;
    class Texture;

    using TexPair = Pair<TextureMeta, Ref<Texture> >;

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
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(uint32_t width, uint32_t height, 
                                    bool MSAAOpened = false,
                                    const char* format = "RGBA8");
        static Ref<Texture2D> Create(const std::string& path, bool gammaCorrection = false);
    };

    class Cubemap : public Texture
    {
    public:
        static Ref<Cubemap> Create(const std::vector<std::string>& facesPath);
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
        std::unordered_map<std::string, TexPair > m_Textures;
    };
}
