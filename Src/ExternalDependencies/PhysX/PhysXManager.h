#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXMANAGER_H_

#include <d3d11.h>

#include "Math/Constant.h"
#include "Utility/Macro.h"
#include "Utility/Memory.h"

#include "PhysXHelper.h"

class PhysXManager
{
public:

    ~PhysXManager() noexcept {
        Release();
    }

    void Init(const DirectX::SimpleMath::Vector3& gravity = { 0.f, -constant::fG, 0.f }, bool enable_cuda = false, ID3D11Device* dev = nullptr) {
        if (INVALID_POINTER(m_pFoundation, PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocator, m_defaultErrorCallback))) {
            return;
        }

        if (VALID_POINTER(m_pPvd, physx::PxCreatePvd(*m_pFoundation))) {
            physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
            m_pPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
        }

        if (INVALID_POINTER(m_pPhysics, PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true, m_pPvd))) {
            return;
        }

        if (!PxInitExtensions(*m_pPhysics, m_pPvd)) {
            return;
        }
        PxCloseExtensions();

        if (INVALID_POINTER(m_pCooking, PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(physx::PxTolerancesScale())))) {
            return;
        }

        m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(4);

        physx::PxSceneDesc scene_desc(m_pPhysics->getTolerancesScale());
        scene_desc.gravity = physx::PxVec3(physx_helper::ToPxVec3(gravity));
        scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;
        scene_desc.cpuDispatcher = m_pDispatcher;

        if (enable_cuda) {
            physx::PxCudaContextManagerDesc cuda_ctx_mgr_desc;
            cuda_ctx_mgr_desc.interopMode = physx::PxCudaInteropMode::D3D11_INTEROP;
            cuda_ctx_mgr_desc.graphicsDevice = dev;

            if (VALID_POINTER(m_pCudaCtxMgr, PxCreateCudaContextManager(*m_pFoundation, cuda_ctx_mgr_desc, PxGetProfilerCallback()))) {
                if (!m_pCudaCtxMgr->contextIsValid()) {
                    memory::SafeRelease(&m_pCudaCtxMgr);
                }
            }

            scene_desc.cudaContextManager = m_pCudaCtxMgr;
            scene_desc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
            scene_desc.flags |= physx::PxSceneFlag::eENABLE_PCM;
            scene_desc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
            scene_desc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
            scene_desc.gpuMaxNumPartitions = 8;
        }

        m_pScene = m_pPhysics->createScene(scene_desc);

        physx::PxPvdSceneClient* pvd_client;
        if (VALID_POINTER(pvd_client, m_pScene->getScenePvdClient())) {
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        m_pMaterials.emplace("default", m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f));

#ifdef _DEBUG
        m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.f);
        m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 1.f);
#endif // _DEBUG

    }

    void Update(double delta_time) {
        m_pScene->simulate(static_cast<physx::PxReal>(delta_time));
        m_pScene->fetchResults(true);
    }

    void AddActor(physx::PxActor* actor) {
        m_pScene->addActor(*actor);
    }

    void AddMaterial(std::string_view material_name, physx::PxMaterial* material) {
        m_pMaterials.emplace(material_name.data(), material);
    }

    // Deprecated
    physx::PxPhysics* GetPhysics() {
        return m_pPhysics;
    }
    physx::PxScene* GetScene() {
        return m_pScene;
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
    physx::PxShape* ConvexMesh(physx::PxConvexMesh* mesh, std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxConvexMeshGeometry(mesh), *FindMaterial(material_name));
    }
    physx::PxShape* TriangleMesh(physx::PxTriangleMesh* mesh, const DirectX::SimpleMath::Vector3& scale = { 1.f, 1.f, 1.f }, std::string_view material_name = "") {
        return m_pPhysics->createShape(physx::PxTriangleMeshGeometry(mesh, physx::PxMeshScale(physx_helper::ToPxVec3(scale))), *FindMaterial(material_name));
    }

    physx::PxRigidStatic* StaticPlane(std::string_view material_name = "") {
        return physx::PxCreatePlane(*m_pPhysics, physx::PxPlane(0, 1, 0, 0), *FindMaterial(material_name));
    }

    template<class VArray>
    physx::PxConvexMesh* ToPxConvexMesh(const VArray& vertices) {
        return physx_helper::ToPxConvexMesh(m_pPhysics, m_pCooking, vertices);
    }
    template<class VArray, class TArray>
    physx::PxTriangleMesh* ToPxTriangleMesh(const VArray& vertices, const TArray& triangles) {
        return physx_helper::ToPxTriangleMesh(m_pPhysics, m_pCooking, vertices, triangles);
    }
    
    physx::PxRigidStatic* CreateStatic(
        const DirectX::SimpleMath::Vector3&    position   = {},
        const DirectX::SimpleMath::Quaternion& quaternion = {}
    ) {
        return physx_helper::CreateStatic(m_pPhysics, physx_helper::ToPxTransform(position, quaternion));
    }
    physx::PxRigidStatic* CreateStatic(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& rotation
    ) {
        return physx_helper::CreateStatic(m_pPhysics, physx_helper::ToPxTransform(position, rotation));
    }
    physx::PxRigidStatic* CreateStatic(
        physx::PxShape*                        shape,
        const DirectX::SimpleMath::Vector3&    local_position   = {},
        const DirectX::SimpleMath::Quaternion& local_quaternion = {},
        const DirectX::SimpleMath::Vector3&    position         = {},
        const DirectX::SimpleMath::Quaternion& quaternion       = {}
    ) {
        physx::PxRigidStatic* rigid_static = physx_helper::CreateStatic(m_pPhysics, physx_helper::ToPxTransform(position, quaternion));
        physx_helper::AttachShape(reinterpret_cast<physx::PxRigidActor**>(&rigid_static), &shape, physx_helper::ToPxTransform(local_position, local_quaternion));
        return rigid_static;
    }

    physx::PxRigidDynamic* CreateDynamic(
        const DirectX::SimpleMath::Vector3&    position   = {},
        const DirectX::SimpleMath::Quaternion& quaternion = {},
        const DirectX::SimpleMath::Vector3&    velocity   = {},
        float                                  damping    = 0.f
    ) {
        return physx_helper::CreateDynamic(m_pPhysics, physx_helper::ToPxTransform(position, quaternion), velocity, damping);
    }
    physx::PxRigidDynamic* CreateDynamic(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& rotation,
        const DirectX::SimpleMath::Vector3& velocity = {},
        float                               damping  = 0.f
    ) {
        return physx_helper::CreateDynamic(m_pPhysics, physx_helper::ToPxTransform(position, rotation), velocity, damping);
    }
    physx::PxRigidDynamic* CreateDynamic(
        physx::PxShape*                        shape,
        const DirectX::SimpleMath::Vector3&    local_position   = {},
        const DirectX::SimpleMath::Quaternion& local_quaternion = {},
        const DirectX::SimpleMath::Vector3&    position         = {},
        const DirectX::SimpleMath::Quaternion& quaternion       = {},
        const DirectX::SimpleMath::Vector3&    velocity         = {},
        float                                  damping          = 0.f
    ) {
        physx::PxRigidDynamic* rigid_dynamic = physx_helper::CreateDynamic(m_pPhysics, physx_helper::ToPxTransform(position, quaternion), velocity, damping);
        physx_helper::AttachShape(reinterpret_cast<physx::PxRigidActor**>(&rigid_dynamic), &shape, physx_helper::ToPxTransform(local_position, local_quaternion));
        return rigid_dynamic;
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

#endif
