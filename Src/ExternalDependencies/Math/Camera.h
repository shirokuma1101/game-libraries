#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_CAMERA_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_CAMERA_H_

#include <cmath>
#include <tuple>

#include "Math/Constant.h"
#include "Math/Convert.h"

#include "SimpleMath.h"
#ifndef CAMERA_DISABLE_LINK_LIBS
#pragma comment(lib, "DirectXTK.lib")
#endif

namespace camera {

    struct Projection {
        Projection() noexcept
            : fov(0.f)
            , aspect(0.f)
            , nearClippingDistance(0.f)
            , farClippingDistance(0.f)
            , matrix(DirectX::SimpleMath::Matrix::Identity)
        {}
        Projection(float fov, float aspect, float near_clipping_distance, float far_clipping_distance) noexcept
            : fov(fov)
            , aspect(aspect)
            , nearClippingDistance(near_clipping_distance)
            , farClippingDistance(far_clipping_distance)
            , matrix(DirectX::XMMatrixPerspectiveFovLH(convert::ToRadians(fov), aspect, nearClippingDistance, farClippingDistance))
        {}

        DirectX::SimpleMath::Matrix Set() noexcept {
            return matrix = DirectX::XMMatrixPerspectiveFovLH(convert::ToRadians(fov), aspect, nearClippingDistance, farClippingDistance);
        }

        //note: https://stackoverflow.com/questions/46182845/field-of-view-aspect-ratio-view-matrix-from-projection-matrix-hmd-ost-calib
        static std::tuple<float, float, float, float> GetProj(const DirectX::SimpleMath::Matrix& proj_mat) {
            float fov                    = convert::ToDegrees(2.0f * std::atan(1.0f / proj_mat._22));
            float aspect                 = proj_mat._22 / proj_mat._11;
            float near_clipping_distance = -(proj_mat._43 / proj_mat._33);
            float far_clipping_distance  = -(proj_mat._43 / (proj_mat._33 - 1));
            return { fov, aspect, near_clipping_distance, far_clipping_distance };
        }

        float                       fov;
        float                       aspect;
        float                       nearClippingDistance;
        float                       farClippingDistance;
        DirectX::SimpleMath::Matrix matrix;
    };

    inline std::tuple<DirectX::SimpleMath::Vector2, bool> WorldToScreen(const DirectX::SimpleMath::Viewport& vp, const DirectX::SimpleMath::Vector3& world_pos, const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat) {
        DirectX::SimpleMath::Matrix convert_mat = DirectX::SimpleMath::Matrix::CreateTranslation(world_pos) * view_mat * proj_mat;

        convert_mat._41 /= convert_mat._44;
        convert_mat._42 /= convert_mat._44;
        convert_mat._43 /= convert_mat._44;

        DirectX::SimpleMath::Vector3 screen_pos = convert_mat.Translation();

        return { { screen_pos.x * convert::ToHalf(vp.width), screen_pos.y * convert::ToHalf(vp.height) }, convert_mat._44 < 0 };
    }

    inline DirectX::SimpleMath::Vector3 ScreenToWorld(const DirectX::SimpleMath::Viewport& vp, const DirectX::SimpleMath::Vector3& screen_pos, const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat) {
        DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateTranslation(screen_pos.x / convert::ToHalf(vp.width), screen_pos.y / convert::ToHalf(vp.height), 0);

        DirectX::SimpleMath::Matrix convert_mat = mat * proj_mat.Invert() * view_mat.Invert();

        convert_mat._41 /= convert_mat._44;
        convert_mat._42 /= convert_mat._44;
        convert_mat._43 /= convert_mat._44;

        return convert_mat.Translation();
    }
    
}

struct Camera {

    Camera(float fov, float aspect, float near_clipping_distance, float far_clipping_distance, DirectX::SimpleMath::Matrix camera_mat) noexcept
        : m_cameraMatrix(camera_mat)
        , m_viewMatrix(m_cameraMatrix.Invert())
        , m_projection(camera::Projection(fov, aspect, near_clipping_distance, far_clipping_distance))
    {}

    void SetMatrix(DirectX::SimpleMath::Matrix camera_mat) {
        m_cameraMatrix = camera_mat;
        m_viewMatrix = m_cameraMatrix.Invert();
    }

    void SetProjection(float fov, float aspect, float near_clipping_distance, float far_clipping_distance) {
        m_projection = camera::Projection(fov, aspect, near_clipping_distance, far_clipping_distance);
    }

    DirectX::SimpleMath::Matrix m_cameraMatrix;
    DirectX::SimpleMath::Matrix m_viewMatrix;
    camera::Projection          m_projection;

};

#endif
