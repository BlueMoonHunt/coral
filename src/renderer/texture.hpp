#include "base.hpp"
#include <filesystem>

typedef unsigned int GLenum;

namespace coral {
    class Texture {
    public:
        Texture(const ivec2_t layout);
        Texture(const std::filesystem::path& path);

        ~Texture();

        ivec2_t getLayout() { return m_Layout; }
        uint32_t getID() { return m_RendererID; }

        void setData(void* data, uint32_t size);
        bool operator==(const Texture& other) const {
            return m_RendererID == other.m_RendererID;
        }
        void bind(uint32_t slot = 0) const;
    private:
        std::filesystem::path m_Path;
        ivec2_t m_Layout;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
} // namespace coral
