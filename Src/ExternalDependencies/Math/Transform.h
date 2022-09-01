#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_TRANSFORM_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_TRANSFORM_H_

#include "Math/Convert.h"

#include "SimpleMath.h"

struct Transform {

    using Vector3    = DirectX::SimpleMath::Vector3;
    using Quaternion = DirectX::SimpleMath::Quaternion;
    using Matrix     = DirectX::SimpleMath::Matrix;

#ifdef TRANSFORM_ROTATION_USE_EULER
    Transform() noexcept
        : position(Vector3::Zero)
        , rotation(Vector3::Zero)
        , scale(1.f)
        , matrix(Matrix::Identity)
    {}

    Transform(const Vector3& p, const Vector3& r, const Vector3& s) noexcept
        : position(p)
        , rotation(r)
        , scale(s)
    {}
    
    // CreateMatrix
    Matrix Composition() & noexcept {
        return matrix = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(convert::ToRadians(rotation)) * Matrix::CreateTranslation(position);
    }

    void Reset() noexcept {
        position = Vector3::Zero;
        rotation = Vector3::Zero;
        scale = { 1.f };
        matrix = Matrix::Identity;
    }
#else
    Transform() noexcept
        : position(Vector3::Zero)
        , quaternion(Quaternion::Identity)
        , scale(1.f)
        , matrix(Matrix::Identity)
    {}

    Transform(const Vector3& p, const Quaternion& q, const Vector3& s) noexcept
        : position(p)
        , quaternion(q)
        , scale(s)
    {}
    
    // CreateMatrix
    Matrix Composition() & noexcept {
        return matrix = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(quaternion) * Matrix::CreateTranslation(position);
    }

    void Reset() noexcept {
        position = Vector3::Zero;
        quaternion = Quaternion::Identity;
        scale = { 1.f };
        matrix = Matrix::Identity;
    }
#endif // TRANSFORM_ROTAION_USE_EULER

    static Transform Identity() noexcept {
        return std::move(Transform());
    }

    static Matrix CreateFromYawPitchRoll(const Vector3 rot) noexcept {
        return Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
    }

    Vector3    position;
    Vector3    scale;
#ifdef TRANSFORM_ROTATION_USE_EULER
    Vector3    rotation;
#else
    Quaternion quaternion;
#endif

    Matrix     matrix;

};

#endif
