#pragma once

#include "Inc/DirectXTK/SimpleMath.h"

struct Transform {

    using Vector3    = DirectX::SimpleMath::Vector3;
    using Quaternion = DirectX::SimpleMath::Quaternion;
    using Matrix     = DirectX::SimpleMath::Matrix;

    Transform() noexcept
        : position(Vector3::Zero)
        , quaternion(Quaternion::Identity)
        , scale(1.f)
        , matrix(Matrix::Identity)
    {}

    Transform(const Vector3& pos, const Quaternion& q, const Vector3& s) noexcept
        : position(pos)
        , quaternion(q)
        , scale(s)
    {}
    
    Vector3    position;
    Quaternion quaternion;
    Vector3    scale;
    Matrix     matrix;

    // CreateMatrix
    Matrix Composition()& noexcept {
        return matrix = Matrix::CreateTranslation(scale) * Matrix::CreateFromQuaternion(quaternion) * Matrix::CreateTranslation(position);
    }

    void Reset() noexcept {
        position = Vector3::Zero;
        quaternion = Quaternion::Identity;
        scale = { 1.f };
        matrix = Matrix::Identity;
    }

    static Transform Identity() noexcept {
        return std::move(Transform());
    }

    static Matrix CreateFromYawPitchRoll(const Vector3 rot) noexcept {
        return Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
    }

};
