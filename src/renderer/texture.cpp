#include "texture.hpp"
#include <glad/glad.h>
#include <stb_image.h>

namespace coral {
    Texture::Texture(const ivec2_t layout)
        : m_Layout(layout) {
        m_InternalFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Layout.x, m_Layout.y);
        
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture::Texture(const std::filesystem::path& path)
    : m_Path(path) {
        ivec2_t size;
        int channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &size.x, &size.y, &channels, 0);
        BMH_ASSERT(data, "failed to load image");
        m_Layout = size;

        GLenum internalFormat = 0, dataFormat = 0;
        switch (channels)
        {
        case 4: internalFormat = GL_RGBA8; dataFormat = GL_RGBA; break;
        case 3: internalFormat = GL_RGB8; dataFormat = GL_RGB; break;
        }
        BMH_ASSERT(internalFormat & dataFormat, "Format not supported");

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Layout.x, m_Layout.y);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Layout.x, m_Layout.y, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::setData(void* data, uint32_t size) {
        BMH_ASSERT(size == (m_Layout.x * m_Layout.y) * (m_DataFormat == GL_RGBA ? 4 : 3) , "data must be entire texture");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Layout.x, m_Layout.y, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture::bind(uint32_t slot) const {
        glBindTextureUnit(slot, m_RendererID);
    }


} // namespace coral
