#include "shader.h"
#include <format>
#include <glad/glad.h>
#include <vector>

#include "core/coralLogger.h"

namespace coral {
    Shader::Shader(const std::string name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
        : m_Name(name) {
        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexShaderSource;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentShaderSource;
        compileShader(shaderSource);
    }

    Shader::~Shader() {
        glDeleteProgram(m_RendererID);
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

    void Shader::setFloat2(const std::string& name, const vec2& value) {
        glUniform2f(getUniformLocation(name), value[0], value[1]);
    }

    void Shader::setFloat3(const std::string& name, const vec3& value) {
        glUniform3f(getUniformLocation(name), value[0], value[1], value[2]);
    }

    void Shader::setFloat4(const std::string& name, const vec4& value) {
        glUniform4f(getUniformLocation(name), value[0], value[1], value[2], value[3]);
    }

    void Shader::setMat3(const std::string& name, const mat3& matrix) {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, matrix.data());
    }

    void Shader::setMat4(const std::string& name, const mat4& matrix) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.data());
    }

    void Shader::compileShader(const std::unordered_map<GLenum, std::string>& shaderSource) {
        GLenum program = glCreateProgram();

        std::vector<GLenum> shaderIDs(shaderSource.size());
        for (auto& kv : shaderSource) {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int result;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE) {
                int length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)malloc(length * sizeof(char));
                glGetShaderInfoLog(shader, length, &length, message);
                glDeleteShader(shader);

                Log::log(message, Log::Error);
                Log::log("shader compilation failure", Log::Error);
                return;
            }
            glAttachShader(program, shader);
            shaderIDs.push_back(shader);
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
            Log::log("Uniform " + name + " doesn't exist", Log::Warn);
        m_UniformLocationCache[name] = location;
        return location;
    }
} // namespace coral