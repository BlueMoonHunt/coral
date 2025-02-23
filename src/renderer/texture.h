#pragma once
#include "core/base.h"

namespace coral {
    using GLenum = unsigned int;

    class Texture2D {
    public:
        Texture2D(const int32_t width, const int32_t height);
        Texture2D(const std::string& path);
        ~Texture2D();

        uint32_t getWidth() const { return m_Width; }
        uint32_t getHeight() const { return m_Height; }
        uint32_t getID() const { return m_RendererID; }

        void setData(void* data, uint32_t size);
        bool operator==(const Texture2D& other) const {
            return m_RendererID == ((Texture2D&)other).m_RendererID;
        }
        void bind(uint32_t slot = 0) const;
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };

} // namespace coral
