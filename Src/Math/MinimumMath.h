#pragma once

#ifndef GAME_LIBRARIES_MATH_MINIMUMMATH_H_
#define GAME_LIBRARIES_MATH_MINIMUMMATH_H_

#include <cmath>

#pragma warning(push)
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union

namespace minimum_math {
    struct Vector3 {
        Vector3()                             noexcept : x(0), y(0), z(0) {}
        Vector3(float s)                      noexcept : x(s), y(s), z(s) {}
        Vector3(float x, float y, float z)    noexcept : x(x), y(y), z(z) {}

        // Unary
        Vector3 operator-()             const noexcept { return Vector3(-x, -y, -z); }
        // Assignment
        Vector3& operator=(const Vector3& v)  noexcept { x = v.x;  y = v.y;  z = v.z;  return *this; }
        // Compound assignment
        Vector3& operator+=(const Vector3& v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        Vector3& operator-=(const Vector3& v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        Vector3& operator*=(float s)          noexcept { x *= s;   y *= s;   z *= s;   return *this; }
        Vector3& operator*=(const Vector3& v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
        Vector3& operator/=(float s)          noexcept { x /= s;   y /= s;   z /= s;   return *this; }
        Vector3& operator/=(const Vector3& v) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }

        float   Dot(const Vector3& v)   const noexcept { return x * v.x + y * v.y + z * v.z; }
        Vector3 Cross(const Vector3& v) const noexcept { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
        
        float   Length()                const noexcept { return std::sqrt(Dot(*this)); }
        float   LengthSquared()         const noexcept { return Dot(*this); }
        
        float x, y, z;
    };
    // Arithmetic
    Vector3 operator+(const Vector3& v1, const Vector3& v2) noexcept { return Vector3(v1) += v2; }
    Vector3 operator-(const Vector3& v1, const Vector3& v2) noexcept { return Vector3(v1) -= v2; }
    Vector3 operator*(const Vector3& v,  float s)           noexcept { return Vector3(v)  *= s;  }
    Vector3 operator*(const Vector3& v1, const Vector3& v2) noexcept { return Vector3(v1) *= v2; }

    struct Matrix4x4 {
        Matrix4x4() noexcept
            : m00(1), m01(0), m02(0), m03(0)
            , m10(0), m11(1), m12(0), m13(0)
            , m20(0), m21(0), m22(1), m23(0)
            , m30(0), m31(0), m32(0), m33(1) {}
        Matrix4x4(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33) noexcept
            : m00(m00), m01(m01), m02(m02), m03(m03)
            , m10(m10), m11(m11), m12(m12), m13(m13)
            , m20(m20), m21(m21), m22(m22), m23(m23)
            , m30(m30), m31(m31), m32(m32), m33(m33) {}

        // Assignment
        Matrix4x4& operator=(const Matrix4x4& m4x4) noexcept {
            m00 = m4x4.m00; m01 = m4x4.m01; m02 = m4x4.m02; m03 = m4x4.m03;
            m10 = m4x4.m10; m11 = m4x4.m11; m12 = m4x4.m12; m13 = m4x4.m13;
            m20 = m4x4.m20; m21 = m4x4.m21; m22 = m4x4.m22; m23 = m4x4.m23;
            m30 = m4x4.m30; m31 = m4x4.m31; m32 = m4x4.m32; m33 = m4x4.m33;
            return *this;
        }
        // Compound assignment
        Matrix4x4& operator+=(const Matrix4x4& m4x4) noexcept {
            m00 += m4x4.m00; m01 += m4x4.m01; m02 += m4x4.m02; m03 += m4x4.m03;
            m10 += m4x4.m10; m11 += m4x4.m11; m12 += m4x4.m12; m13 += m4x4.m13;
            m20 += m4x4.m20; m21 += m4x4.m21; m22 += m4x4.m22; m23 += m4x4.m23;
            m30 += m4x4.m30; m31 += m4x4.m31; m32 += m4x4.m32; m33 += m4x4.m33;
            return *this;
        }
        Matrix4x4& operator-=(const Matrix4x4& m4x4) noexcept {
            m00 -= m4x4.m00; m01 -= m4x4.m01; m02 -= m4x4.m02; m03 -= m4x4.m03;
            m10 -= m4x4.m10; m11 -= m4x4.m11; m12 -= m4x4.m12; m13 -= m4x4.m13;
            m20 -= m4x4.m20; m21 -= m4x4.m21; m22 -= m4x4.m22; m23 -= m4x4.m23;
            m30 -= m4x4.m30; m31 -= m4x4.m31; m32 -= m4x4.m32; m33 -= m4x4.m33;
            return *this;
        }
        Matrix4x4& operator*=(float s) noexcept {
            m00 *= s; m01 *= s; m02 *= s; m03 *= s;
            m10 *= s; m11 *= s; m12 *= s; m13 *= s;
            m20 *= s; m21 *= s; m22 *= s; m23 *= s;
            m30 *= s; m31 *= s; m32 *= s; m33 *= s;
            return *this;
        }

#ifdef RIGHT_HANDED
        Vector3 Up()                 const noexcept { return Vector3( m20,  m21,  m22); }
        Vector3 Down()               const noexcept { return Vector3(-m20, -m21, -m22); }
        Vector3 Right()              const noexcept { return Vector3( m10,  m11,  m12); }
        Vector3 Left()               const noexcept { return Vector3(-m10, -m11, -m12); }
        Vector3 Forward()            const noexcept { return Vector3( m00,  m01,  m02); }
        Vector3 Backward()           const noexcept { return Vector3(-m00, -m01, -m02); }
        void    Up(const Vector3& v)       noexcept { m20 =  v.x; m21 =  v.y; m22 =  v.z; }
        void    Down(const Vector3& v)     noexcept { m20 = -v.x; m21 = -v.y; m22 = -v.z; }
        void    Right(const Vector3& v)    noexcept { m10 =  v.x; m11 =  v.y; m12 =  v.z; }
        void    Left(const Vector3& v)     noexcept { m10 = -v.x; m11 = -v.y; m12 = -v.z; }
        void    Forward(const Vector3& v)  noexcept { m00 =  v.x; m01 =  v.y; m02 =  v.z; }
        void    Backward(const Vector3& v) noexcept { m00 = -v.x; m01 = -v.y; m02 = -v.z; }
#else
        Vector3 Up()                    const noexcept { return Vector3( m10,  m11,  m12); }
        Vector3 Down()                  const noexcept { return Vector3(-m10, -m11, -m12); }
        Vector3 Right()                 const noexcept { return Vector3( m00,  m01,  m02); }
        Vector3 Left()                  const noexcept { return Vector3(-m00, -m01, -m02); }
        Vector3 Forward()               const noexcept { return Vector3(-m20, -m21, -m22); }
        Vector3 Backward()              const noexcept { return Vector3( m20,  m21,  m22); }
        void    Up(const Vector3& v)          noexcept { m10 =  v.x; m11 =  v.y; m12 =  v.z; }
        void    Down(const Vector3& v)        noexcept { m10 = -v.x; m11 = -v.y; m12 = -v.z; }
        void    Right(const Vector3& v)       noexcept { m00 =  v.x; m01 =  v.y; m02 =  v.z; }
        void    Left(const Vector3& v)        noexcept { m00 = -v.x; m01 = -v.y; m02 = -v.z; }
        void    Forward(const Vector3& v)     noexcept { m20 = -v.x; m21 = -v.y; m22 = -v.z; }
        void    Backward(const Vector3& v)    noexcept { m20 =  v.x; m21 =  v.y; m22 =  v.z; }
#endif
        Vector3 Translation()           const noexcept { return Vector3( m30,  m31,  m32); }
        void    Translation(const Vector3& v) noexcept { m30 =  v.x; m31 =  v.y; m32 =  v.z; }

        union {
            struct {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            float m[4][4];
        };
    };
    // Arithmetic
    Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) noexcept { return Matrix4x4(m1) += m2; }
    Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) noexcept { return Matrix4x4(m1) -= m2; }
    Matrix4x4 operator*(const Matrix4x4& m,  float s)             noexcept { return Matrix4x4(m)  *= s;  }
    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) noexcept {
        Matrix4x4 m;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                for (int inner = 0; inner < 4; ++inner) {
                    m.m[row][col] += m1.m[row][inner] * m2.m[inner][col];
                }
            }
        }
        return m;
    }
}

#pragma warning(pop)

#endif
