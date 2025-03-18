#include <array>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <iostream>
#include <stdexcept>

template<typename T>
struct Vec2 {
    T x, y;
    Vec2() : x(), y() {}
    Vec2(T _x, T _y) : x(_x), y(_y) {}
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    Vec2 operator*(T scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    friend Vec2 operator*(T scalar, const Vec2& v) {
        return v * scalar;
    }
    Vec2 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        return Vec2(x / scalar, y / scalar);
    }
    T dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }
    T length() const {
        return std::sqrt(x * x + y * y);
    }
    Vec2 normalize() const {
        T len = length();
        if (len == static_cast<T>(0)) {
            return *this;
        }
        return *this / len;
    }
    Vec2 element_wise_multiply(const Vec2& other) const {
        return Vec2(x * other.x, y * other.y);
    }
    Vec2 element_wise_divide(const Vec2& other) const {
        if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0)) {
            throw std::runtime_error("Element-wise division by zero!");
        }
        return Vec2(x / other.x, y / other.y);
    }
    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }
};

template<typename T>
struct Vec3 {
    T x, y, z;
    Vec3() : x(), y(), z() {}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    Vec3 operator*(T scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    friend Vec3 operator*(T scalar, const Vec3& v) {
        return v * scalar;
    }
    Vec3 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
    T dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Vec3 cross(const Vec3& other) const {
        return Vec3(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }
    T length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    Vec3 normalize() const {
        T len = length();
        if (len == static_cast<T>(0)) {
            return *this;
        }
        return *this / len;
    }
    Vec3 element_wise_multiply(const Vec3& other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    Vec3 element_wise_divide(const Vec3& other) const {
        if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0)) {
            throw std::runtime_error("Element-wise division by zero!");
        }
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }
};

template<typename T>
struct Vec4 {
    T x, y, z, w;
    Vec4() : x(), y(), z(), w() {}
    Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vec4 operator+(const Vec4& other) const {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vec4 operator-(const Vec4& other) const {
        return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vec4 operator*(T scalar) const {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    friend Vec4 operator*(T scalar, const Vec4& v) {
        return v * scalar;
    }
    Vec4 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    T dot(const Vec4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    T length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    Vec4 normalize() const {
        T len = length();
        if (len == static_cast<T>(0)) {
            return *this;
        }
        return *this / len;
    }
    Vec4 element_wise_multiply(const Vec4& other) const {
        return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }
    Vec4 element_wise_divide(const Vec4& other) const {
        if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0) || other.w == static_cast<T>(0)) {
            throw std::runtime_error("Element-wise division by zero!");
        }
        return Vec4(x / other.x, y / other.y, z / other.z, w / other.w);
    }
    bool operator==(const Vec4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    bool operator!=(const Vec4& other) const {
        return !(*this == other);
    }
};

template<typename T>
struct Mat2 {
    std::array<T, 4> data;
    Mat2() : data() {}
    Mat2(T m00, T m01, T m10, T m11) : data({ m00, m01, m10, m11 }) {}
    T& operator()(size_t r, size_t c) {
        return data[r * 2 + c];
    }
    const T& operator()(size_t r, size_t c) const {
        return data[r * 2 + c];
    }
    Mat2 operator+(const Mat2& other) const {
        Mat2 result;
        for (size_t i = 0; i < 4; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }
    Mat2 operator-(const Mat2& other) const {
        Mat2 result;
        for (size_t i = 0; i < 4; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }
    Mat2 operator*(T scalar) const {
        Mat2 result;
        for (size_t i = 0; i < 4; ++i) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }
    friend Mat2 operator*(T scalar, const Mat2& m) {
        return m * scalar;
    }
    Mat2 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        Mat2 result;
        for (size_t i = 0; i < 4; ++i) {
            result.data[i] = data[i] / scalar;
        }
        return result;
    }
    Mat2 operator*(const Mat2& other) const {
        Mat2 result;
        result(0, 0) = data[0] * other(0, 0) + data[1] * other(1, 0);
        result(0, 1) = data[0] * other(0, 1) + data[1] * other(1, 1);
        result(1, 0) = data[2] * other(0, 0) + data[3] * other(1, 0);
        result(1, 1) = data[2] * other(0, 1) + data[3] * other(1, 1);
        return result;
    }
    template <typename U>
    Vec2<T> operator*(const Vec2<U>& vec) const {
        return Vec2<T>(data[0] * vec.x + data[1] * vec.y,
            data[2] * vec.x + data[3] * vec.y);
    }
    Mat2 transpose() const {
        return Mat2(data[0], data[2], data[1], data[3]);
    }
    T determinant() const {
        return data[0] * data[3] - data[1] * data[2];
    }
    Mat2 inverse() const {
        T det = determinant();
        if (det == static_cast<T>(0)) {
            throw std::runtime_error("Matrix is not invertible (determinant is zero)!");
        }
        T invDet = static_cast<T>(1) / det;
        return Mat2(data[3] * invDet, -data[1] * invDet, -data[2] * invDet, data[0] * invDet);
    }
    bool operator==(const Mat2& other) const {
        return data == other.data;
    }
    bool operator!=(const Mat2& other) const {
        return !(*this == other);
    }

};

template<typename T>
struct Mat3 {
    std::array<T, 9> data;
    Mat3() : data() {}
    Mat3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) : data({ m00, m01, m02, m10, m11, m12, m20, m21, m22 }) {}
    T& operator()(size_t r, size_t c) {
        return data[r * 3 + c];
    }
    const T& operator()(size_t r, size_t c) const {
        return data[r * 3 + c];
    }
    Mat3 operator+(const Mat3& other) const {
        Mat3 result;
        for (size_t i = 0; i < 9; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }
    Mat3 operator-(const Mat3& other) const {
        Mat3 result;
        for (size_t i = 0; i < 9; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }
    Mat3 operator*(T scalar) const {
        Mat3 result;
        for (size_t i = 0; i < 9; ++i) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }
    friend Mat3 operator*(T scalar, const Mat3& m) {
        return m * scalar;
    }
    Mat3 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        Mat3 result;
        for (size_t i = 0; i < 9; ++i) {
            result.data[i] = data[i] / scalar;
        }
        return result;
    }
    Mat3 operator*(const Mat3& other) const {
        Mat3 result;
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                T sum = static_cast<T>(0);
                for (size_t k = 0; k < 3; ++k) {
                    sum += data[i * 3 + k] * other.data[k * 3 + j];
                }
                result.data[i * 3 + j] = sum;
            }
        }
        return result;
    }
    template <typename U>
    Vec3<T> operator*(const Vec3<U>& vec) const {
        return Vec3<T>(data[0] * vec.x + data[1] * vec.y + data[2] * vec.z,
            data[3] * vec.x + data[4] * vec.y + data[5] * vec.z,
            data[6] * vec.x + data[7] * vec.y + data[8] * vec.z);
    }
    Mat3 transpose() const {
        return Mat3(data[0], data[3], data[6],
            data[1], data[4], data[7],
            data[2], data[5], data[8]);
    }
    T determinant() const {
        return data[0] * (data[4] * data[8] - data[5] * data[7]) -
            data[1] * (data[3] * data[8] - data[5] * data[6]) +
            data[2] * (data[3] * data[7] - data[4] * data[6]);
    }
    Mat3 inverse() const {
        T det = determinant();
        if (det == static_cast<T>(0)) {
            throw std::runtime_error("Matrix is not invertible (determinant is zero)!");
        }
        T invDet = static_cast<T>(1) / det;
        return Mat3((data[4] * data[8] - data[5] * data[7]) * invDet,
            -(data[1] * data[8] - data[2] * data[7]) * invDet,
            (data[1] * data[5] - data[2] * data[4]) * invDet,
            -(data[3] * data[8] - data[5] * data[6]) * invDet,
            (data[0] * data[8] - data[2] * data[6]) * invDet,
            -(data[0] * data[5] - data[2] * data[3]) * invDet,
            (data[3] * data[7] - data[4] * data[6]) * invDet,
            -(data[0] * data[7] - data[1] * data[6]) * invDet,
            (data[0] * data[4] - data[1] * data[3]) * invDet);
    }
    bool operator==(const Mat3& other) const {
        return data == other.data;
    }
    bool operator!=(const Mat3& other) const {
        return !(*this == other);
    }

};

template<typename T>
struct Mat4 {
    std::array<T, 16> data;
    Mat4() : data() {}
    Mat4(T m00, T m01, T m02, T m03,
        T m10, T m11, T m12, T m13,
        T m20, T m21, T m22, T m23,
        T m30, T m31, T m32, T m33) : data({ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 }) {
    }
    T& operator()(size_t r, size_t c) {
        return data[r * 4 + c];
    }
    const T& operator()(size_t r, size_t c) const {
        return data[r * 4 + c];
    }
    Mat4 operator+(const Mat4& other) const {
        Mat4 result;
        for (size_t i = 0; i < 16; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }
    Mat4 operator-(const Mat4& other) const {
        Mat4 result;
        for (size_t i = 0; i < 16; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }
    Mat4 operator*(T scalar) const {
        Mat4 result;
        for (size_t i = 0; i < 16; ++i) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }
    friend Mat4 operator*(T scalar, const Mat4& m) {
        return m * scalar;
    }
    Mat4 operator/(T scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Scalar division by zero!");
        }
        Mat4 result;
        for (size_t i = 0; i < 16; ++i) {
            result.data[i] = data[i] / scalar;
        }
        return result;
    }
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                T sum = static_cast<T>(0);
                for (size_t k = 0; k < 4; ++k) {
                    sum += data[i * 4 + k] * other.data[k * 4 + j];
                }
                result.data[i * 4 + j] = sum;
            }
        }
        return result;
    }
    template <typename U>
    Vec4<T> operator*(const Vec4<U>& vec) const {
        return Vec4<T>(data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3] * vec.w,
            data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7] * vec.w,
            data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11] * vec.w,
            data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w);
    }
    Mat4 transpose() const {
        return Mat4(data[0], data[4], data[8], data[12],
            data[1], data[5], data[9], data[13],
            data[2], data[6], data[10], data[14],
            data[3], data[7], data[11], data[15]);
    }
    T determinant() const {
        T det = static_cast<T>(0);
        Mat3<T> submatrix;
        auto get_submatrix = [&](size_t col) {
            size_t sub_r = 0;
            for (size_t r = 1; r < 4; ++r) {
                size_t sub_c = 0;
                for (size_t c = 0; c < 4; ++c) {
                    if (c != col) {
                        submatrix(sub_r, sub_c) = data[r * 4 + c];
                        sub_c++;
                    }
                }
                sub_r++;
            }
            return submatrix;
            };
        det += data[0] * get_submatrix(0).determinant();
        det -= data[1] * get_submatrix(1).determinant();
        det += data[2] * get_submatrix(2).determinant();
        det -= data[3] * get_submatrix(3).determinant();
        return det;
    }
    Mat4 inverse() const {
        T det = determinant();
        if (det == static_cast<T>(0)) {
            throw std::runtime_error("Matrix is not invertible (determinant is zero)!");
        }
        T invDet = static_cast<T>(1) / det;
        Mat4 adjugate;
        auto cofactor = [&](size_t r, size_t c) {
            Mat3<T> submatrix;
            size_t sub_r = 0;
            for (size_t i = 0; i < 4; ++i) {
                if (i != r) {
                    size_t sub_c = 0;
                    for (size_t j = 0; j < 4; ++j) {
                        if (j != c) {
                            submatrix(sub_r, sub_c) = data[i * 4 + j];
                            sub_c++;
                        }
                    }
                    sub_r++;
                }
            }
            T det_sub = submatrix.determinant();
            return ((r + c) % 2 == 0) ? det_sub : -det_sub;
            };
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                adjugate(j, i) = cofactor(i, j);
            }
        }
        return adjugate * invDet;
    }
    bool operator==(const Mat4& other) const {
        return data == other.data;
    }
    bool operator!=(const Mat4& other) const {
        return !(*this == other);
    }
};

using vec2_t = Vec2<float>;
using vec3_t = Vec3<float>;
using vec4_t = Vec4<float>;
using mat2_t = Mat2<float>;
using mat3_t = Mat3<float>;
using mat4_t = Mat4<float>;

using dvec2_t = Vec2<double>;
using dvec3_t = Vec3<double>;
using dvec4_t = Vec4<double>;
using dmat2_t = Mat2<double>;
using dmat3_t = Mat3<double>;
using dmat4_t = Mat4<double>;

using i8vec2_t = Vec2<int8_t>;
using i8vec3_t = Vec3<int8_t>;
using i8vec4_t = Vec4<int8_t>;
using i16vec2_t = Vec2<int16_t>;
using i16vec3_t = Vec3<int16_t>;
using i16vec4_t = Vec4<int16_t>;
using i32vec2_t = Vec2<int32_t>;
using i32vec3_t = Vec3<int32_t>;
using i32vec4_t = Vec4<int32_t>;
using ssvec2_t = Vec2<ssize_t>;
using ssvec3_t = Vec3<ssize_t>;
using ssvec4_t = Vec4<ssize_t>;

using u8vec2_t = Vec2<uint8_t>;
using u8vec3_t = Vec3<uint8_t>;
using u8vec4_t = Vec4<uint8_t>;
using u16vec2_t = Vec2<uint16_t>;
using u16vec3_t = Vec3<uint16_t>;
using u16vec4_t = Vec4<uint16_t>;
using u32vec2_t = Vec2<uint32_t>;
using u32vec3_t = Vec3<uint32_t>;
using u32vec4_t = Vec4<uint32_t>;
using svec2_t = Vec2<size_t>;
using svec3_t = Vec3<size_t>;
using svec4_t = Vec4<size_t>;
using smat2_t = Mat2<size_t>;
using smat3_t = Mat3<size_t>;
using smat4_t = Mat4<size_t>;

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec4<T>& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Mat2<T>& m) {
    os << m(0, 0) << " " << m(0, 1) << "\n"
        << m(1, 0) << " " << m(1, 1);
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Mat3<T>& m) {
    os << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << "\n"
        << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << "\n"
        << m(2, 0) << " " << m(2, 1) << " " << m(2, 2);
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Mat4<T>& m) {
    os << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << " " << m(0, 3) << "\n"
        << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << " " << m(1, 3) << "\n"
        << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << " " << m(2, 3) << "\n"
        << m(3, 0) << " " << m(3, 1) << " " << m(3, 2) << " " << m(3, 3);
    return os;
}