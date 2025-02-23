#include "buffer.h"
#include <glad/glad.h>
#include <print>
#include "core/coralLogger.h"

namespace coral {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type)
        {
        case ShaderDataType::Float:        return GL_FLOAT;
        case ShaderDataType::Float2:       return GL_FLOAT;
        case ShaderDataType::Float3:       return GL_FLOAT;
        case ShaderDataType::Float4:       return GL_FLOAT;
        case ShaderDataType::Mat3:         return GL_FLOAT;
        case ShaderDataType::Mat4:         return GL_FLOAT;
        case ShaderDataType::Int:          return GL_INT;
        case ShaderDataType::Int2:         return GL_INT;
        case ShaderDataType::Int3:         return GL_INT;
        case ShaderDataType::Int4:         return GL_INT;
        case ShaderDataType::UInt:         return GL_UNSIGNED_INT;
        case ShaderDataType::UInt2:        return GL_UNSIGNED_INT;
        case ShaderDataType::UInt3:        return GL_UNSIGNED_INT;
        case ShaderDataType::UInt4:        return GL_UNSIGNED_INT;
        case ShaderDataType::Bool:         return GL_BOOL;
        case ShaderDataType::UByte:        return GL_UNSIGNED_BYTE;
        case ShaderDataType::Byte:         return GL_BYTE;
        }

        Log::log("Unknown ShaderDataType", Log::Error);
        return 0;
    }

    uint32_t BufferElement::GetComponentCount() const {
        {
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

            Log::log("Unknown ShaderDataType", Log::Error);
            return 0;
        }
    }

    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count) {
        glCreateBuffers(1,&m_RendererID);
        glNamedBufferData(m_RendererID,count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }


    VertexBuffer::VertexBuffer(uint32_t size) {
        glCreateVertexArrays(1, &m_VA_RendererID);

        glCreateBuffers(1, &m_VB_RendererID);
        glNamedBufferData(m_VB_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    }
    VertexBuffer::VertexBuffer(const float* data, uint32_t size) {
        glCreateVertexArrays(1, &m_VA_RendererID);

        glCreateBuffers(1, &m_VB_RendererID);
        glNamedBufferData(m_VB_RendererID, size, data, GL_STATIC_DRAW);
    }
    VertexBuffer::~VertexBuffer() {
        glDeleteVertexArrays(1, &m_VA_RendererID);
        glDeleteBuffers(1, &m_VB_RendererID);
    }
    void VertexBuffer::setData(const void* data, uint32_t size) {
        glNamedBufferSubData(m_VB_RendererID, 0, size, data);
    }
    void VertexBuffer::setIndexBuffer(Ref<IndexBuffer> indexBuffer) {
        glVertexArrayElementBuffer(m_VA_RendererID, indexBuffer->getID());
        m_IndexBuffer = indexBuffer;
    }
    void VertexBuffer::setLayout(const BufferLayout& layout) {
        m_Layout = layout;
        if (m_Layout.getElements().empty())
            Log::log("Vertex Buffer has no layout", Log::Error);

        uint32_t index = 0;
        for (const auto& element : m_Layout) {
            glEnableVertexArrayAttrib(m_VA_RendererID, index);
            glVertexArrayAttribBinding(m_VA_RendererID, index, 0);
            glVertexArrayAttribFormat(m_VA_RendererID, index, element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayVertexBuffer(m_VA_RendererID, index, m_VB_RendererID, element.Offset, m_Layout.getStride());
            index++;
        }
    }
} // namespace coral   