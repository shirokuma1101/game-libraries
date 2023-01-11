#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_OPENVR_OPENVRMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_OPENVR_OPENVRMANAGER_H_

#include "OpenVRHelper.h"

class OpenVRManager
{
public:

    bool Init(std::string* return_error = nullptr) {
        // check if OpenVR is installed
        if (!vr::VR_IsHmdPresent()) {
            ErrorDetail("OpenVR is not installed.", return_error);
            return false;
        }

        // check if OpenVR is running
        if (!vr::VR_IsRuntimeInstalled()) {
            ErrorDetail("OpenVR is not running.", return_error);
            return false;
        }

        // initialize OpenVR
        vr::EVRInitError error = vr::VRInitError_None;
        m_pHMD = vr::VR_Init(&error, vr::VRApplication_Scene);
        if (error != vr::VRInitError_None) {
            ErrorDetail("OpenVR failed to initialize. Error: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)), return_error);
            return false;
        }

        // initialize OpenVR render models
        m_pRenderModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
        if (!m_pRenderModels) {
            ErrorDetail("OpenVR failed to initialize render models. Error: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)), return_error);
            return false;
        }

        // check if OpenVR is ready
        if (!vr::VRCompositor()) {
            ErrorDetail("OpenVR failed to initialize compositor.", return_error);
            return false;
        }

        return true;
    }

    void Update() {
        vr::VRCompositor()->WaitGetPoses(m_trackedDevicePoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
    }

    void Draw(Texture2D* texture_left_eye, Texture2D* texture_right_eye) {
        vr::texture_t texture_left = { (void*)texture_left_eye, vr::TextureType_DirectX, vr::ColorSpace_Auto };
        vr::VRCompositor()->Submit(vr::Eye_Left, &texture_left);
        vr::texture_t texture_right = { (void*)texture_right_eye, vr::TextureType_DirectX, vr::ColorSpace_Auto };
        vr::VRCompositor()->Submit(vr::Eye_Right, &texture_right);
        vr::VRCompositor()->PostPresentHandoff();
    }

    void Release() {
        if (m_pHMD) {
            vr::VR_Shutdown();
            m_pHMD = nullptr;
        }
    }

    DirectX::SimpleMath::Matrix GetTrackedDevicePose(vr::TrackedDeviceIndex_t device_index) const noexcept {
        if (m_trackedDevicePoses[device_index].bPoseIsValid) {
            return OpenVRHelper::ConvertMatrix(m_trackedDevicePoses[device_index].mDeviceToAbsoluteTracking);
        }
        return DirectX::SimpleMath::Matrix::Identity;
    }

    std::list<DirectX::SimpleMath::Matrix> GetTrackedDevicePoses(vr::TrackedDeviceClass tracked_device_class) const noexcept {
        std::list<DirectX::SimpleMath::Matrix> poses;
        for (vr::TrackedDeviceIndex_t device_index = 0; device_index < vr::k_unMaxTrackedDeviceCount; ++device_index) {
            if (m_pHMD && m_pHMD->GetTrackedDeviceClass(device_index) == tracked_device_class) {
                poses.push_back(GetTrackedDevicePose(device_index));
            }
        }
        return poses;
    }

private:

    void ErrorDetail(st::string_view error, std::string* return_error = nullptr) {
        if (return_error) {
            *return_error = error;
        }
        assert::ShowWarning(ASSERT_FILE_LINE, error);
    }

    vr::IVRSystem*          m_pHMD          = nullptr;
    vr::IVRRenderModels*    m_pRenderModels = nullptr;
    vr::TrackedDevicePose_t m_trackedDevicePoses[vr::k_unMaxTrackedDeviceCount];

};

#endif
