#include "shader.hpp"
#include <glad/glad.h>
#include <vector>
#include <print>

namespace coral {
    static GLenum ShaderTypeFromString(const std::string& type) {
        if (type == "vertex_Shader")
            return GL_VERTEX_SHADER;
        else if (type == "fragment_Shader" || type == "pixel_Shader")
            return GL_FRAGMENT_SHADER;

        BMH_ASSERT(false, "Unknown shader type");
        return 0;
    }

    Shader::Shader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
        : m_Name(name) {
        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexShaderSource;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentShaderSource;
        compileShader(shaderSource);
    }

    Shader::~Shader() {
        glDeleteShader(m_RendererID);
    }

    void Shader::bind() const {
        glUseProgram(m_RendererID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setInt(const std::string& name, int value) {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setIntArray(const std::string& name, int* values, uint32_t count) {
        glUniform1iv(getUniformLocation(name), count, values);
    }

    void Shader::setFloat(const std::string& name, float value) {
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setFloat2(const std::string& name, const vec2_t& value) {
        glUniform2f(getUniformLocation(name), value.x, value.y);
    }

    void Shader::setFloat3(const std::string& name, const vec3_t& value) {
        glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::setFloat4(const std::string& name, const vec4_t& value) {
        glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::setMat3(const std::string& name, const mat3_t& value) {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, value.data.data());
    }

    void Shader::setMat4(const std::string& name, const mat4_t& value) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.data.data());
    }


    void Shader::compileShader(const std::unordered_map<GLenum, std::string>& shaderSource) {
        GLenum program = glCreateProgram();
        std::vector<GLenum> shaderIDs(shaderSource.size());

        for (auto& pair : shaderSource) {
            GLenum type = pair.first;
            const std::string& source = pair.second;

            GLenum shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int result;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
            if (result == GL_FALSE) {
                int length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(shader, length, &length, message);
                glDeleteShader(shader);

                BMH_ERROR(message);
                BMH_ASSERT(false, "shader compilation failure");
                return;
            }
            glAttachShader(program, shader);
            shaderIDs.emplace_back(shader);
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            //TO CHANGE

            glDeleteProgram(program);

            for (auto id : shaderIDs)
                glDeleteShader(id);
        }

        for (GLenum id : shaderIDs) {
            glDetachShader(program, id);
        }

        m_RendererID = program;
    }

    int32_t Shader::getUniformLocation(const std::string& name) {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::println("Warning: Uniform {} doesn't exist", name);
        m_UniformLocationCache[name] = location;
        return location;
    }

} // namespace coral