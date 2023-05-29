#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXHELPER_H_

#include "Math/Convert.h"

#pragma warning(push)
#pragma warning(disable:26495) // Variable 'identifier' is uninitialized. Always initialize a member variable (type.6).
#include "PxPhysicsAPI.h"
#pragma warning(pop)
#include "PxFoundation.h"
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXTask_static_64.lib")
#pragma comment(lib, "SceneQuery_static_64.lib")
#pragma comment(lib, "SimulationController_static_64.lib")
#include "SimpleMath.h"

namespace physx_helper {

    /**************************************************
    * Math convert
    **************************************************/

    /* DirectX Simple Math to PhysX math */

    inline physx::PxMat44 ToPxMat44(const DirectX::SimpleMath::Matrix& mat) {
        return physx::PxMat44(
            physx::PxVec4(mat._11, mat._12, mat._13, mat._14),
            physx::PxVec4(mat._21, mat._22, mat._23, mat._24),
            physx::PxVec4(mat._31, mat._32, mat._33, mat._34),
            physx::PxVec4(mat._41, mat._42, mat._43, mat._44)
        );
    }

    inline physx::PxVec3 ToPxVec3(const DirectX::SimpleMath::Vector3& vec3) {
        return physx::PxVec3(vec3.x, vec3.y, vec3.z);
    }

    inline physx::PxVec4 ToPxVec4(const DirectX::SimpleMath::Vector4& vec4) {
        return physx::PxVec4(vec4.x, vec4.y, vec4.z, vec4.w);
    }

    inline physx::PxQuat ToPxQuat(const DirectX::SimpleMath::Quaternion& quat) {
        return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
    }

    inline physx::PxTransform ToPxTransform(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& quaternion) {
        return physx::PxTransform(ToPxVec3(position), ToPxQuat(quaternion));
    }
    inline physx::PxTransform ToPxTransform(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation) {
        return physx::PxTransform(ToPxVec3(position), ToPxQuat(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(convert::ToRadians(rotation))));
    }

    /* PhysX math to DirectX Simple Math */

    inline DirectX::SimpleMath::Matrix ToMatrix(const physx::PxMat44& px_mat44) noexcept {
        return DirectX::SimpleMath::Matrix(
            px_mat44.column0.x, px_mat44.column0.y, px_mat44.column0.z, px_mat44.column0.w,
            px_mat44.column1.x, px_mat44.column1.y, px_mat44.column1.z, px_mat44.column1.w,
            px_mat44.column2.x, px_mat44.column2.y, px_mat44.column2.z, px_mat44.column2.w,
            px_mat44.column3.x, px_mat44.column3.y, px_mat44.column3.z, px_mat44.column3.w
        );
    }

    inline DirectX::SimpleMath::Vector3 ToVector3(const physx::PxVec3& px_vec3) noexcept {
        return DirectX::SimpleMath::Vector3(px_vec3.x, px_vec3.y, px_vec3.z);
    }


    /****************************************
    * PhysX helper
    ****************************************/

    template<class VArray>
    inline physx::PxConvexMesh* ToPxConvexMesh(physx::PxPhysics* physics, physx::PxCooking* cooking, const VArray& vertices) {
        physx::PxConvexMeshDesc px_mesh_desc;
        px_mesh_desc.points.count  = static_cast<physx::PxU32>(vertices.size());
        px_mesh_desc.points.stride = sizeof(VArray::value_type);
        px_mesh_desc.points.data   = &vertices[0];
        px_mesh_desc.flags         = physx::PxConvexFlag::eCOMPUTE_CONVEX;

        physx::PxDefaultMemoryOutputStream write_buffer;
        cooking->cookConvexMesh(px_mesh_desc, write_buffer);
        physx::PxDefaultMemoryInputData read_buffer(write_buffer.getData(), write_buffer.getSize());
        physx::PxConvexMesh* convex_mesh = physics->createConvexMesh(read_buffer);
        return convex_mesh;
    }

    template<class VArray, class TArray>
    inline physx::PxTriangleMesh* ToPxTriangleMesh(physx::PxPhysics* physics, physx::PxCooking* cooking, const VArray& vertices, const TArray& triangles) {
        physx::PxTriangleMeshDesc px_mesh_desc;
        px_mesh_desc.setToDefault();
        px_mesh_desc.points.count     = static_cast<physx::PxU32>(vertices.size());
        px_mesh_desc.points.stride    = sizeof(VArray::value_type);
        px_mesh_desc.points.data      = &vertices[0];
        px_mesh_desc.triangles.count  = static_cast<physx::PxU32>(triangles.size());
        px_mesh_desc.triangles.stride = sizeof(TArray::value_type);
        px_mesh_desc.triangles.data   = &triangles[0];

        physx::PxDefaultMemoryOutputStream write_buffer;
        cooking->cookTriangleMesh(px_mesh_desc, write_buffer);
        physx::PxDefaultMemoryInputData read_buffer(write_buffer.getData(), write_buffer.getSize());
        physx::PxTriangleMesh* triangle_mesh = physics->createTriangleMesh(read_buffer);
        return triangle_mesh;
    }

    inline std::vector<physx::PxShape*> GetShapes(physx::PxRigidActor* actor) {
        std::vector<physx::PxShape*> shapes;
        physx::PxU32 shape_count = actor->getNbShapes();
        physx::PxShape** ppshapes = new physx::PxShape*[shape_count];
        shapes.reserve(shape_count);

        actor->getShapes(ppshapes, shape_count);
        for (physx::PxU32 i = 0; i < shape_count; ++i) {
            shapes.push_back(ppshapes[i]);
        }
        delete[] ppshapes;
        return shapes;
    }

    inline void AttachShape(
        physx::PxRigidActor**     actor,
        physx::PxShape**          shape,
        const physx::PxTransform& local_transform = physx::PxTransform(physx::PxIdentity)
    ) {
        (*shape)->setLocalPose(local_transform);
        (*actor)->attachShape(**shape);
    }

    inline physx::PxRigidStatic* CreateStatic(
        physx::PxPhysics*         physics,
        const physx::PxTransform& transform = physx::PxTransform(physx::PxIdentity)
    ) {
        return physics->createRigidStatic(transform);
    }

    inline physx::PxRigidDynamic* CreateDynamic(
        physx::PxPhysics*         physics,
        const physx::PxTransform& transform = physx::PxTransform(physx::PxIdentity)
    ) {
        return physics->createRigidDynamic(transform);
    }

    inline physx::PxRigidDynamic* IsDynamic(physx::PxRigidActor* actor) {
        if (physx::PxRigidDynamic* dynamic = actor->is<physx::PxRigidDynamic>(); dynamic) {
            return dynamic;
        }
        return nullptr;
    }

    inline bool IsSleeping(physx::PxRigidActor* actor) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(actor); dynamic) {
            return dynamic->isSleeping();
        }
        return false;
    }

    inline void PutToSleep(physx::PxRigidActor* actor) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(actor); dynamic) {
            dynamic->putToSleep();
        }
    }
    inline void WakeUp(physx::PxRigidActor* actor) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(actor); dynamic) {
            dynamic->wakeUp();
        }
    }

    //note: "https://nekopro99.com/move-rigidbody-addforce/"
    //note: "https://ekulabo.com/force-mode"
    inline void AddForce(physx::PxRigidActor* actor, const DirectX::SimpleMath::Vector3& force, physx::PxForceMode::Enum mode = physx::PxForceMode::eFORCE) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(actor); dynamic) {
            dynamic->isSleeping();
            dynamic->addForce(physx_helper::ToPxVec3(force), mode);
        }
    }

    inline DirectX::SimpleMath::Vector3 CalcCG(physx::PxRigidActor* actor, const DirectX::SimpleMath::Vector3& offset = {}) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(actor); dynamic) {
            physx::PxU32 shape_count = dynamic->getNbShapes();
            physx::PxShape** shapes = new physx::PxShape*[shape_count];

            physx::PxVec3 cg = dynamic->getCMassLocalPose().p;
            dynamic->getShapes(shapes, shape_count);
            for (physx::PxU32 i = 0; i < shape_count; ++i) {
                physx::PxShape* shape = shapes[i];
                physx::PxVec3 local = shape->getLocalPose().p;
                cg += local;
            }
            delete[] shapes;

            return (ToVector3(cg) / static_cast<float>(shape_count)) + offset;
        }
        return DirectX::SimpleMath::Vector3();
    }

    inline void CalcCG(physx::PxRigidActor** actor, const DirectX::SimpleMath::Vector3& offset = {}) {
        if (physx::PxRigidDynamic* dynamic = IsDynamic(*actor); dynamic) {
            DirectX::SimpleMath::Vector3 cg = CalcCG(*actor, offset);
            dynamic->setCMassLocalPose(physx::PxTransform(ToPxVec3(cg)));
        }
    }

    class RigidActorHolder
    {
    public:

        RigidActorHolder(physx::PxRigidActor* actor)
            : m_pActor(actor)
            , m_moveVector()
            , m_beforePosition(physx_helper::ToVector3(actor->getGlobalPose().p))
        {}

        void Update() {
            auto now_position = physx_helper::ToVector3(m_pActor->getGlobalPose().p);
            m_moveVector = now_position - m_beforePosition;
            m_beforePosition = now_position;
        }

        physx::PxRigidActor* GetRigidActor() noexcept {
            return m_pActor;
        }

        const DirectX::SimpleMath::Vector3& GetMoveVector() const noexcept {
            return m_moveVector;
        }

        DirectX::SimpleMath::Matrix GetMatrix() const {
            return ToMatrix(m_pActor->getGlobalPose());
        }

    private:

        physx::PxRigidActor*         m_pActor = nullptr;
        DirectX::SimpleMath::Vector3 m_moveVector;
        DirectX::SimpleMath::Vector3 m_beforePosition;

    };

}

#endif
