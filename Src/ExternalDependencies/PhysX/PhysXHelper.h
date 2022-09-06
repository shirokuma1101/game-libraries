#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXHELPER_H_

#include "PxPhysicsAPI.h"
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

    inline physx::PxVec3 ToPxVec3(const DirectX::SimpleMath::Vector3& vec3) {
        return physx::PxVec3(vec3.x, vec3.y, vec3.z);
    }

    inline physx::PxQuat ToPxQuat(const DirectX::SimpleMath::Quaternion& quat) {
        return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
    }

    inline DirectX::SimpleMath::Matrix ToMatrix(const physx::PxMat44& px_mat44) {
        return DirectX::SimpleMath::Matrix(
            px_mat44.column0.x, px_mat44.column0.y, px_mat44.column0.z, px_mat44.column0.w,
            px_mat44.column1.x, px_mat44.column1.y, px_mat44.column1.z, px_mat44.column1.w,
            px_mat44.column2.x, px_mat44.column2.y, px_mat44.column2.z, px_mat44.column2.w,
            px_mat44.column3.x, px_mat44.column3.y, px_mat44.column3.z, px_mat44.column3.w
        );
    }

    inline DirectX::SimpleMath::Vector3 ToVector3(const physx::PxVec3& px_vec3) {
        return DirectX::SimpleMath::Vector3(static_cast<float>(px_vec3.x), static_cast<float>(px_vec3.y), static_cast<float>(px_vec3.z));
    }

    template<class VArray>
    inline physx::PxConvexMesh* ToPxConvexMesh(physx::PxPhysics* physics, physx::PxCooking* cooking, const VArray& vertices) {
        physx::PxConvexMeshDesc px_mesh_desc;
        px_mesh_desc.points.count = static_cast<physx::PxU32>(vertices.size());;
        px_mesh_desc.points.stride = sizeof(VArray::value_type);
        px_mesh_desc.points.data = &vertices[0];;
        px_mesh_desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

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

    //TODO: PxQuat

    inline void AttachShape(physx::PxShape** shape, physx::PxRigidActor** actor, const DirectX::SimpleMath::Vector3& local_position = {}) {
        (*shape)->setLocalPose(physx::PxTransform(physx_helper::ToPxVec3(local_position)));
        (*actor)->attachShape(**shape);
    }

    inline physx::PxRigidStatic* CreateStatic(
        physx::PxPhysics* physics,
        const DirectX::SimpleMath::Vector3& position = {}
    ) {
        return physics->createRigidStatic(physx::PxTransform(ToPxVec3(position)));
    }

    inline physx::PxRigidDynamic* CreateDynamic(
        physx::PxPhysics* physics,
        const DirectX::SimpleMath::Vector3& position = {},
        const DirectX::SimpleMath::Vector3& velocity = {},
        float damping = 0.f
    ) {
        physx::PxRigidDynamic* rigid_dynamic = physics->createRigidDynamic(physx::PxTransform(ToPxVec3(position)));
        rigid_dynamic->setLinearVelocity(ToPxVec3(velocity));
        //rigid_dynamic->setAngularVelocity(ToPxVec3(velocity));
        //rigid_dynamic->setLinearDamping(damping);
        rigid_dynamic->setAngularDamping(damping);
        return rigid_dynamic;
    }

    inline physx::PxRigidDynamic* IsDynamic(physx::PxRigidActor* actor) {
        physx::PxRigidDynamic* dynamic = actor->is<physx::PxRigidDynamic>();
        if (!dynamic) {
            // "actor is not dynamic assert"
            return nullptr;
        }
        return dynamic;
    }

    inline DirectX::SimpleMath::Vector3 CalcCG(physx::PxRigidActor* actor, const DirectX::SimpleMath::Vector3& offset = {}) {
        physx::PxRigidDynamic* dynamic = IsDynamic(actor);
        if (!dynamic) {
            return DirectX::SimpleMath::Vector3();
        }

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

    inline void CalcCG(physx::PxRigidActor** actor, const DirectX::SimpleMath::Vector3& offset = {}) {
        physx::PxRigidDynamic* dynamic = IsDynamic(*actor);
        DirectX::SimpleMath::Vector3 cg = CalcCG(*actor);
        dynamic->setCMassLocalPose(physx::PxTransform(ToPxVec3(cg)));
    }

}

#endif
