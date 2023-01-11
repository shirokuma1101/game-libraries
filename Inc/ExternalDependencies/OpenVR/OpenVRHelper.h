#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_OPENVR_OPENVRHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_OPENVR_OPENVRHELPER_H_

#include <memory>

#include "openvr.h"
#pragma comment(lib, "openvr_api.lib")

#include "SimpleMath.h"

namespace openvr_helper {

    inline DirectX::SimpleMath::Matrix ToMatrix(const vr::HmdMatrix34_t& mat) {
        return DirectX::SimpleMath::Matrix(
            mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0f,
            mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0f,
            mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0f,
            mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
        );
    }

    inline DirectX::SimpleMath::Matrix ToMatrix(const vr::HmdMatrix44_t& mat) {
        return DirectX::SimpleMath::Matrix(
            mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
            mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
            mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
            mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
        );
    }

    inline DirectX::SimpleMath::Matrix GetLHProjectionMatrix(vr::IVRSystem* hmd, vr::Hmd_Eye eye, float near, float far) {
        if (!hmd) {
            return DirectX::SimpleMath::Matrix::Identity;
        }
        DirectX::SimpleMath::Matrix mat = ToMatrix(hmd->GetProjectionMatrix(eye, near, far));
        mat._33 = far / (far - near);
        mat._43 = -near * mat._33;
        return mat;
    }

    inline std::pair<int32_t, int32_t> GetRecommendedRenderTargetSize(vr::IVRSystem* hmd) {
        if (!hmd) {
            return std::make_pair(0, 0);
        }
        uint32_t width = 0;
        uint32_t height = 0;
        hmd->GetRecommendedRenderTargetSize(&width, &height);
        return std::make_pair(static_cast<int32_t>(width), static_cast<int32_t>(height));
    }

    // ?
    inline DirectX::SimpleMath::Matrix GetLHViewMatrix(vr::IVRSystem* hmd, vr::Hmd_Eye eye) {
        if (!hmd) {
            return DirectX::SimpleMath::Matrix::Identity;
        }
        return ToMatrix(hmd->GetEyeToHeadTransform(eye)).Invert();
    }

}

#endif
