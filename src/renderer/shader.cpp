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

        CORAL_LOG("Unknown shader type", LogLevel::FETAL);
        return 0;
    }

    Shader::Shader(const std::string& _name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
        : name(_name) {
        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexShaderSource;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentShaderSource;
        compileShader(shaderSource);
    }

    Shader::~Shader() {
        glDeleteShader(rendererID);
    }

    void Shader::bind() const {
        glUseProgram(rendererID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setInt(const std::string& _name, int value) {
        glUniform1i(getUniformLocation(_name), value);
    }

    void Shader::setIntArray(const std::string& _name, int* values, uint32_t count) {
        glUniform1iv(getUniformLocation(_name), count, values);
    }

    void Shader::setFloat(const std::string& _name, float value) {
        glUniform1f(getUniformLocation(_name), value);
    }

    void Shader::setFloat2(const std::string& _name, const vec2_t& value) {
        glUniform2f(getUniformLocation(_name), value.x, value.y);
    }

    void Shader::setFloat3(const std::string& _name, const vec3_t& value) {
        glUniform3f(getUniformLocation(_name), value.x, value.y, value.z);
    }

    void Shader::setFloat4(const std::string& _name, const vec4_t& value) {
        glUniform4f(getUniformLocation(_name), value.x, value.y, value.z, value.w);
    }

    void Shader::setMat3(const std::string& _name, const mat3_t& value) {
        glUniformMatrix3fv(getUniformLocation(_name), 1, GL_FALSE, value.data.data());
    }

    void Shader::setMat4(const std::string& _name, const mat4_t& value) {
        glUniformMatrix4fv(getUniformLocation(_name), 1, GL_FALSE, value.data.data());
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

                CORAL_LOG(message, LogLevel::ERROR);
                CORAL_LOG("shader compilation failure", LogLevel::FETAL);
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

        rendererID = program;
    }

    int32_t Shader::getUniformLocation(const std::string& _name) {
        if (uniformLocationCache.find(_name) != uniformLocationCache.end())
            return uniformLocationCache[_name];
        int32_t location = glGetUniformLocation(rendererID, _name.c_str());
        if (location == -1)
            std::println("Warning: Uniform {} doesn't exist", _name);
        uniformLocationCache[_name] = location;
        return location;
    }

} // namespace coral