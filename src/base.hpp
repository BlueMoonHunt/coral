#pragma once
#include <memory>
#include <array>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <csignal>

// math implementation

template<typename T>
struct Vec2 {
    T x, y;
};

template<typename T>
struct Vec3 {
    T x, y, z;
};

template<typename T>
struct Vec4 {
    T x, y, z, w;
};

using vec2 = Vec2<float>;
using vec3 = Vec3<float>;
using vec4 = Vec4<float>;

using dvec2 = Vec2<double>;
using dvec3 = Vec3<double>;
using dvec4 = Vec4<double>;

using ivec2 = Vec2<int32_t>;
using ivec3 = Vec3<int32_t>;
using ivec4 = Vec4<int32_t>;

using uvec2 = Vec2<uint32_t>;
using uvec3 = Vec3<uint32_t>;
using uvec4 = Vec4<uint32_t>;

enum CoralApplicationFlag {
    CoralApplicationFlag_None = 0,
    CoralApplicationFlag_Fullscreen = 1 << 0,
    CoralApplicationFlag_Iconified = 1 << 1,
    CoralApplicationFlag_Resizable = 1 << 2,
    CoralApplicationFlag_NoBorder = 1 << 3,
    CoralApplicationFlag_Debug = 1 << 4,

};

enum CoralRendererFlag {
    CoralRendererFlag_None = 0,
    CoralRendererFlag_Batch = 1 << 0,
    CoralRendererFlag_Discrete = 1 << 1,
    CoralRendererFlag_RecreateSwapChain = 1 << 2,
};

namespace coral {
    using FLAG = int;
} // namespace coral