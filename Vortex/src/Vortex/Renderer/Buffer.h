#pragma once

#include <string>

namespace Vortex 
{
    class Texture2D;
    class Texture;
    class Cubemap;

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        }

        VT_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3 * 3;
            case ShaderDataType::Mat4:    return 4 * 4;
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            }

            VT_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        // eg: {element0: a_Position(3 floats), element1: a_Color(4 floats)}
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        inline uint32_t GetStride() const { return m_Stride; }
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> Create(uint32_t size);
        // vertices: the data of vertices stored in buffer
        // size: the size of all vertices
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t count) = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t count);
        // indices: the data of indices stored in buffer
        // count: the count of indices (not size!)
        // the data has been transported to opengl, so you can free the memory of indices
        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

    class RenderBuffer
    {
    public:
        virtual ~RenderBuffer() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetID() const = 0;

        static Ref<RenderBuffer> Create(uint32_t width, uint32_t height);
        static Ref<RenderBuffer> Create(uint32_t width, uint32_t height, int nSamples);
    };

    enum class FrameBufferState
    {
        NONE,
        READ,
        DRAW,
        BOTH
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() {}

        virtual void Bind(FrameBufferState state = FrameBufferState::BOTH) const = 0;
        virtual void Unbind() const = 0;
        
        virtual void AttachRenderBuffer(const Ref<RenderBuffer>& rb) const = 0;
        virtual void AttachTexture2D(Texture2D& tex2D, int attachIndex) = 0;
        virtual void AttachCubemap(Cubemap& cubemap, int attachIndex, int faceIndex) = 0;
        virtual bool CheckStatus() const = 0;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetID() const = 0;

        static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
        
        static void Blit(FrameBuffer& src, FrameBuffer& dst);
    };
}
