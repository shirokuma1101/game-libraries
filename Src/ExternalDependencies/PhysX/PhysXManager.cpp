#include "PhysXManager.h"

#include <Utility/Macro.h>
#include <Utility/Memory.h>

void PhysXManager::Init(DirectX::SimpleMath::Vector3 gravity, bool enable_cuda, ID3D11Device* dev)
{
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
    scene_desc.gravity       = physx::PxVec3(physx_helper::ToPxVec3(gravity));
    scene_desc.filterShader  = physx::PxDefaultSimulationFilterShader;
    scene_desc.cpuDispatcher = m_pDispatcher;

    if (enable_cuda) {
        physx::PxCudaContextManagerDesc cuda_ctx_mgr_desc;
        cuda_ctx_mgr_desc.interopMode    = physx::PxCudaInteropMode::D3D11_INTEROP;
        cuda_ctx_mgr_desc.graphicsDevice = dev;

        if (VALID_POINTER(m_pCudaCtxMgr, PxCreateCudaContextManager(*m_pFoundation, cuda_ctx_mgr_desc, PxGetProfilerCallback()))) {
            if (!m_pCudaCtxMgr->contextIsValid()) {
                memory::SafeRelease(m_pCudaCtxMgr);
            }
        }

        scene_desc.cudaContextManager    = m_pCudaCtxMgr;
        scene_desc.broadPhaseType        = physx::PxBroadPhaseType::eGPU;
        scene_desc.flags                 |= physx::PxSceneFlag::eENABLE_PCM;
        scene_desc.flags                 |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
        scene_desc.flags                 |= physx::PxSceneFlag::eENABLE_STABILIZATION;
        scene_desc.gpuMaxNumPartitions   = 8;
    }

    m_pScene = m_pPhysics->createScene(scene_desc);

    physx::PxPvdSceneClient* pvd_client;
    if (VALID_POINTER(pvd_client, m_pScene->getScenePvdClient())) {
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
}

void PhysXManager::Update(double delta_time)
{
    m_pScene->simulate(static_cast<physx::PxReal>(delta_time));
    m_pScene->fetchResults(true);
}

void PhysXManager::AddActor(physx::PxActor& actor)
{
    m_pScene->addActor(actor);
}

void PhysXManager::Release() noexcept
{
    memory::SafeRelease(m_pScene);
    memory::SafeRelease(m_pDispatcher);
    memory::SafeRelease(m_pCudaCtxMgr);
    memory::SafeRelease(m_pCooking);
    memory::SafeRelease(m_pPhysics);
    if (m_pPvd) {
        m_pPvd->disconnect();
        physx::PxPvdTransport* transport = m_pPvd->getTransport();
        memory::SafeRelease(m_pPvd);
        memory::SafeRelease(transport);
    }
    memory::SafeRelease(m_pPvd);
    memory::SafeRelease(m_pFoundation);
}
