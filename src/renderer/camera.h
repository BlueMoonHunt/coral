#pragma once
#include "core/base.h"

namespace coral
{
    class Camera {
    public:
        enum class ProjectionType : uint8_t {
            orthographic, perspective
        };
    public:
        Camera() = default;
        Camera(const mat4& projection)
            : m_Projection(projection) {}
        const mat4& getProjection() const { return m_Projection; }
    protected:
        mat4 m_Projection;
    };
} // namespace coral
