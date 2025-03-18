#include "base.hpp"
#include <string>
#include <filesystem>
#include <unordered_map>

typedef unsigned int GLenum;

namespace coral {
    class Shader {
    public:
        //TODO: Shader(ShaderComponent); with ECS
        Shader(const std::string& _name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        ~Shader();

        void bind() const;
        void unbind() const;
        const std::string getName() const { return name; }
        //Set Uniform
        void setInt(const std::string& _name, int value);
        void setIntArray(const std::string& _name, int* values, uint32_t count);
        void setFloat(const std::string& _name, float value);
        void setFloat2(const std::string& _name, const vec2_t& value);
        void setFloat3(const std::string& _name, const vec3_t& value);
        void setFloat4(const std::string& _name, const vec4_t& value);
        void setMat3(const std::string& _name, const mat3_t& value);
        void setMat4(const std::string& _name, const mat4_t& value);

    private:
        uint32_t rendererID;
        std::string name;
        std::unordered_map<std::string, int32_t> uniformLocationCache;

        void  compileShader(const std::unordered_map<GLenum, std::string>& shaderSource);
        int32_t getUniformLocation(const std::string& _name);
    };
} // namespace coral
