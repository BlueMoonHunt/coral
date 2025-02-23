#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include "core/base.h"

namespace coral {
    enum class ShaderDataType {
        None = 0, Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4,
        Bool, UByte, Byte
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::UInt:     return 4;
        case ShaderDataType::UInt2:    return 4 * 2;
        case ShaderDataType::UInt3:    return 4 * 3;
        case ShaderDataType::UInt4:    return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Bool:     return 1;
        case ShaderDataType::UByte:    return 1;
        case ShaderDataType::Byte:     return 1;
        }
        return 0;
    }

    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            :Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
        }

        uint32_t GetComponentCount() const;
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            :m_Elements(elements) {
            calculateOffsetAndStride();
        }

        uint32_t getStride() const { return m_Stride; }
        const std::vector<BufferElement>& getElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void calculateOffsetAndStride() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    struct IndexBuffer {
    public:
        IndexBuffer(const uint32_t* data, uint32_t count);
        ~IndexBuffer();
        uint32_t getCount() const { return m_Count; }
        uint32_t getID() const { return m_RendererID; }

    private:
        uint32_t m_Count;
        uint32_t m_RendererID;
    };

    struct VertexBuffer {
    public:
        VertexBuffer(uint32_t size);
        VertexBuffer(const float* data, uint32_t size);
        ~VertexBuffer();

        void setData(const void* data, uint32_t size);
        void setIndexBuffer(Ref<IndexBuffer> indexBuffer);
        void setLayout(const BufferLayout& layout);

        Ref<IndexBuffer> getIndexBuffer() const { return m_IndexBuffer; }
        const BufferLayout& getLayout() const { return m_Layout; }
        uint32_t getID() const { return m_VA_RendererID; }
    private:
        uint32_t m_VB_RendererID;
        uint32_t m_VA_RendererID;
        BufferLayout m_Layout;
        Ref<IndexBuffer> m_IndexBuffer;
    };

} // namespace coral
