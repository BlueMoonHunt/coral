#pragma once
#include <memory>
#include <array>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat3 = std::array<float, 9>;
using mat4 = std::array<float, 16>;

typedef int ApplicationFlag;
typedef int RenderFlag;
typedef int BufferFlag;
typedef int TextureFlag;
typedef int ShaderFlag;


enum ApplicationFlag_ {
    ApplicationFlag_None = 0,
    ApplicationFlag_Debug = 1 << 0,
    ApplicationFlag_Fullscreen = 1 << 1,
    ApplicationFlag_NoResize = 1 << 2,
    ApplicationFlag_VSync = 1 << 3,
    ApplicationFlag_CustomTitleBar = 1 << 4,
};

enum RenderFlag_ {
    RenderFlag_None = 0,
    RenderFlag_CullFaceCCW = 1 << 0,
    RenderFlag_CullFaceCW = 1 << 1,
};