#pragma once

#include <d3d11.h>

#include <Math/Constant.h>
#include "PhysXHelper.h"

class PhysXManager
{
public:

    ~PhysXManager() noexcept {
        Release();
    }

    void Init(DirectX::SimpleMath::Vector3 gravity = { 0.f, constant::fG, 0.f }, bool enable_cuda = false, ID3D11Device* dev = nullptr);
    void Update(double delta_time);

    void AddActor(physx::PxActor& actor);
    
private:

    void Release() noexcept;

    physx::PxDefaultAllocator      m_defaultAllocator;
    physx::PxDefaultErrorCallback  m_defaultErrorCallback;

    physx::PxFoundation*           m_pFoundation = nullptr;
    physx::PxPhysics*              m_pPhysics    = nullptr;
    physx::PxCooking*              m_pCooking    = nullptr;

    physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
    physx::PxScene*                m_pScene      = nullptr;

    physx::PxPvd*                  m_pPvd        = nullptr;

    // cuda
    physx::PxCudaContextManager*   m_pCudaCtxMgr = nullptr;

};
