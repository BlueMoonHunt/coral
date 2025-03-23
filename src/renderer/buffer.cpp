#include "buffer.hpp"
#include <glad/glad.h>

namespace coral {
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
        case coral::ShaderDataType::Float:        return GL_FLOAT;
        case coral::ShaderDataType::Float2:       return GL_FLOAT;
        case coral::ShaderDataType::Float3:       return GL_FLOAT;
        case coral::ShaderDataType::Float4:       return GL_FLOAT;
        case coral::ShaderDataType::Mat3:         return GL_FLOAT;
        case coral::ShaderDataType::Mat4:         return GL_FLOAT;
        case coral::ShaderDataType::Int:          return GL_INT;
        case coral::ShaderDataType::Int2:         return GL_INT;
        case coral::ShaderDataType::Int3:         return GL_INT;
        case coral::ShaderDataType::Int4:         return GL_INT;
        case coral::ShaderDataType::UInt:         return GL_UNSIGNED_INT;
        case coral::ShaderDataType::UInt2:        return GL_UNSIGNED_INT;
        case coral::ShaderDataType::UInt3:        return GL_UNSIGNED_INT;
        case coral::ShaderDataType::UInt4:        return GL_UNSIGNED_INT;
        case coral::ShaderDataType::Bool:         return GL_BOOL;
        case coral::ShaderDataType::UByte:        return GL_UNSIGNED_BYTE;
        case coral::ShaderDataType::Byte:         return GL_BYTE;
        }

        BMH_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    //////////////////////////////////////////
    //            VertexBuffer              //
    //////////////////////////////////////////


    VertexBuffer::VertexBuffer(std::vector<VertexData>& vertices) {
        glCreateVertexArrays(1, &m_VertexArrayID);

        glCreateBuffers(1, &m_VertexBufferID);
        glNamedBufferData(m_VertexBufferID, vertices.size(), vertices.data(), GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(float* data, uint32_t size) {
        glCreateVertexArrays(1, &m_VertexArrayID);

        glCreateBuffers(1, &m_VertexBufferID);
        glNamedBufferData(m_VertexBufferID, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(uint32_t size) {
        glCreateVertexArrays(1, &m_VertexArrayID);

        glCreateBuffers(1, &m_VertexBufferID);
        glNamedBufferData(m_VertexBufferID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteVertexArrays(1, &m_VertexArrayID);
        glDeleteBuffers(1, &m_VertexBufferID);
    }

    void VertexBuffer::setData(const void* data, uint32_t size) {
        glNamedBufferSubData(m_VertexBufferID, 0, size, data);
    }

    void VertexBuffer::setIndexBuffer(Ref<IndexBuffer>& buffer) {
        glVertexArrayElementBuffer(m_VertexArrayID, buffer->getID());
        m_IndexBuffer = buffer;
    }

    const Ref<IndexBuffer>& VertexBuffer::getIndexBuffer() const {
        return m_IndexBuffer;
    }

    const BufferLayout& VertexBuffer::getLayout() const {
        return m_BufferLayout;
    }

    void VertexBuffer::setLayout(const BufferLayout& layout) {
        m_BufferLayout = layout;

        if (!m_BufferLayout.getElements().size())
        BMH_ASSERT(false, "Vertex Buffer has no layout");

        uint32_t index = 0;
        for (const auto& element : m_BufferLayout) {
            glEnableVertexArrayAttrib(m_VertexArrayID, index);
            glVertexArrayAttribBinding(m_VertexArrayID, index, 0);
            glVertexArrayAttribFormat(m_VertexArrayID, index, element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayVertexBuffer(m_VertexArrayID, index, m_VertexBufferID, element.Offset, m_BufferLayout.getStride());
            index++;
        }
    }

    uint32_t VertexBuffer::getID() const {
        return m_VertexArrayID;
    }

    //////////////////////////////////////////
    //            Index Buffer              //
    //////////////////////////////////////////

    IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count)
        : m_Count(count) {
            glCreateBuffers(1, &m_ID);
            glNamedBufferData(m_ID, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_ID);
    }

    uint32_t IndexBuffer::GetCount() const {
        return m_Count;
    }

    uint32_t IndexBuffer::getID() const {
        return m_ID;
    }

} // namespace coral
