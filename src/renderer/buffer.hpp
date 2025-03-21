#include "base.hpp"
#include <vector>

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

        CORAL_LOG("Unknown ShaderDataType!", LogLevel::FETAL);
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

        uint32_t GetComponentCount() const {
            switch (Type) {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3; // 3* float3
            case ShaderDataType::Mat4:    return 4; // 4* float4
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::UInt:    return 1;
            case ShaderDataType::UInt2:   return 2;
            case ShaderDataType::UInt3:   return 3;
            case ShaderDataType::UInt4:   return 4;
            case ShaderDataType::Bool:    return 1;
            case ShaderDataType::UByte:   return 1;
            case ShaderDataType::Byte:    return 1;
            }

            CORAL_LOG("Unknown ShaderDataType!", LogLevel::FETAL);
            return 0;
        }
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

    struct VertexData {
        vec3_t position;
        vec4_t color = vec4_t{1.0f,1.0f,1.0f,1.0f};
        vec2_t texCoord;
        float texIndex = 0.0f;
        float tilingFactor = 1.0f;
    };

    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* data, uint32_t count);
        ~IndexBuffer();
        uint32_t GetCount() const;
        uint32_t getID() const;
    private:
        uint32_t count;
        uint32_t id;
    };


    class VertexBuffer {
    public:
        VertexBuffer(std::vector<VertexData>& vertices);
        VertexBuffer(float* data, uint32_t size);
        VertexBuffer(uint32_t size);
        ~VertexBuffer();

        void setData(const void* data, uint32_t size);
        void setIndexBuffer(Ref<IndexBuffer>& buffer);
        const Ref<IndexBuffer>& getIndexBuffer() const;
        const BufferLayout& getLayout() const;
        void setLayout(const BufferLayout& layout);

        uint32_t getID() const;

    private:
        uint32_t vertexBufferID;
        uint32_t vertexArrayID;
        BufferLayout bufferLayout;
        Ref<IndexBuffer> indexBuffer;
    };
} // namespace coral
