#pragma once

#include <d3d11.h>

#include <Math/Constant.h>
#include <Utility/Macro.h>
#include <Utility/Memory.h>
#include "PhysXHelper.h"

class PhysXManager
{
public:

    ~PhysXManager() noexcept {
        Release();
    }

    void Init(DirectX::SimpleMath::Vector3 gravity = { 0.f, -constant::fG, 0.f }, bool enable_cuda = false, ID3D11Device* dev = nullptr);

    void Update(double delta_time) {
        m_pScene->simulate(static_cast<physx::PxReal>(delta_time));
        m_pScene->fetchResults(true);
    }

    void AddActor(physx::PxActor& actor) {
        m_pScene->addActor(actor);
    }

    void AddMaterial(std::string_view material_name, physx::PxMaterial* material) {
        m_pMaterials.emplace(material_name.data(), material);
    }

    // Deprecated
    physx::PxPhysics* GetPhysics() {
        return m_pPhysics;
    }

    /* Geometry manual */
    // URL:"https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html"
    
    physx::PxShape* Sphere(float radius, std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxSphereGeometry(radius), *FindMaterial(material_name));
    }
    physx::PxShape* Capsule(float radius, float half_height, std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxCapsuleGeometry(radius, half_height), *FindMaterial(material_name));
    }
    physx::PxShape* Box(const DirectX::SimpleMath::Vector3& half_extent, std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxBoxGeometry(physx_helper::ToPxVec3(half_extent)), *FindMaterial(material_name));
    }
    physx::PxShape* Plane(std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxPlaneGeometry(), *FindMaterial(material_name));
    }

    physx::PxRigidStatic* CreateStatic(physx::PxShape* shape, const DirectX::SimpleMath::Vector3& position) {
        return physx_helper::CreateStatic(m_pPhysics, shape, position);
    }
    physx::PxRigidDynamic* CreateDynamic(physx::PxShape* shape, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity = { 0.f, 0.f, 0.f }, float damping = 0.f) {
        return physx_helper::CreateDynamic(m_pPhysics, shape, position);
    }
    
private:

    void Release() noexcept {
        memory::SafeRelease(&m_pScene);
        memory::SafeRelease(&m_pDispatcher);
        memory::SafeRelease(&m_pCudaCtxMgr);
        memory::SafeRelease(&m_pCooking);
        memory::SafeRelease(&m_pPhysics);
        if (m_pPvd) {
            m_pPvd->disconnect();
            physx::PxPvdTransport* transport = m_pPvd->getTransport();
            memory::SafeRelease(&m_pPvd);
            memory::SafeRelease(&transport);
        }
        memory::SafeRelease(&m_pFoundation);
    }

    physx::PxMaterial* FindMaterial(std::string_view material_name) {
        if (auto iter = m_pMaterials.find(material_name.data()); iter != m_pMaterials.end()) {
            return iter->second;
        }
        else {
            return m_pMaterials.at("default");
        }
    }

    physx::PxDefaultAllocator      m_defaultAllocator;
    physx::PxDefaultErrorCallback  m_defaultErrorCallback;

    physx::PxFoundation*           m_pFoundation = nullptr;
    physx::PxPhysics*              m_pPhysics    = nullptr;
    physx::PxCooking*              m_pCooking    = nullptr;

    physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
    physx::PxScene*                m_pScene      = nullptr;

    physx::PxPvd*                  m_pPvd        = nullptr;

    // material
    std::unordered_map<std::string, physx::PxMaterial*> m_pMaterials;

    // cuda
    physx::PxCudaContextManager*   m_pCudaCtxMgr = nullptr;

};
