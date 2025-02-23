#pragma once
#include <unordered_map>
#include "core/base.h"

namespace coral {
    enum class ShaderType : uint8_t {
        VertexShader = 0,
        FragmentShader
    };

    class Shader {
    public:
        Shader(const std::string name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        ~Shader();

        void bind() const;
        void unbind() const;
        const std::string getName() const { return m_Name; }
        //Set Uniform
        void setInt(const std::string& name, int value);
        void setIntArray(const std::string& name, int* values, uint32_t count);
        void setFloat(const std::string& name, float value);
        void setFloat2(const std::string& name, const vec2& value);
        void setFloat3(const std::string& name, const vec3& value);
        void setFloat4(const std::string& name, const vec4& value);
        void setMat3(const std::string& name, const mat3& value);
        void setMat4(const std::string& name, const mat4& value);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
        std::unordered_map<std::string, int32_t> m_UniformLocationCache;
        using GLenum = unsigned int;
        void  compileShader(const std::unordered_map<GLenum, std::string>& shaderSource);
        int32_t getUniformLocation(const std::string& name);
    };
} // namespace coral
