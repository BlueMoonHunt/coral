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

        CORAL_LOG("Unknown ShaderDataType", LogLevel::FETAL);
        return 0;
    }

    //////////////////////////////////////////
    //            VertexBuffer              //
    //////////////////////////////////////////


    VertexBuffer::VertexBuffer(std::vector<VertexData>& vertices) {
        glCreateVertexArrays(1, &vertexArrayID);

        glCreateBuffers(1, &vertexBufferID);
        glNamedBufferData(vertexBufferID, vertices.size(), vertices.data(), GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(float* data, uint32_t size) {
        glCreateVertexArrays(1, &vertexArrayID);

        glCreateBuffers(1, &vertexBufferID);
        glNamedBufferData(vertexBufferID, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(uint32_t size) {
        glCreateVertexArrays(1, &vertexArrayID);

        glCreateBuffers(1, &vertexBufferID);
        glNamedBufferData(vertexBufferID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteBuffers(1, &vertexBufferID);
    }

    void VertexBuffer::setData(const void* data, uint32_t size) {
        glNamedBufferSubData(vertexBufferID, 0, size, data);
    }

    void VertexBuffer::setIndexBuffer(Ref<IndexBuffer>& buffer) {
        glVertexArrayElementBuffer(vertexArrayID, buffer->getID());
        indexBuffer = buffer;
    }

    const Ref<IndexBuffer>& VertexBuffer::getIndexBuffer() const {
        return indexBuffer;
    }

    const BufferLayout& VertexBuffer::getLayout() const {
        return bufferLayout;
    }

    void VertexBuffer::setLayout(const BufferLayout& layout) {
        bufferLayout = layout;

        if (!bufferLayout.getElements().size())
            CORAL_LOG("Vertex Buffer has no layout", LogLevel::FETAL);

        uint32_t index = 0;
        for (const auto& element : bufferLayout) {
            glEnableVertexArrayAttrib(vertexArrayID, index);
            glVertexArrayAttribBinding(vertexArrayID, index, 0);
            glVertexArrayAttribFormat(vertexArrayID, index, element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayVertexBuffer(vertexArrayID, index, vertexBufferID, element.Offset, bufferLayout.getStride());
            index++;
        }
    }

    uint32_t VertexBuffer::getID() const {
        return vertexArrayID;
    }

    //////////////////////////////////////////
    //            Index Buffer              //
    //////////////////////////////////////////

    IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count)
        : count(count) {
            glCreateBuffers(1, &id);
            glNamedBufferData(id, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &id);
    }

    uint32_t IndexBuffer::GetCount() const {
        return count;
    }

    uint32_t IndexBuffer::getID() const {
        return id;
    }

} // namespace coral
