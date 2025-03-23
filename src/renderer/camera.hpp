#pragma once
#include "base.hpp"

namespace coral {
    class Camera {
    public:
        enum class ProjectionType : uint8_t {
            orthographic, perspective
        };
    public:
        Camera() = default;
        Camera(const mat4_t& projection)
            : m_Projection(projection) {
        }
        const mat4_t& getProjection() const { return m_Projection; }
    protected:
        mat4_t m_Projection = mat4_t(1.0f);
    };
}