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
        return DirectX::SimpleMath::Vector3(px_vec3.x, px_vec3.y, px_vec3.z);
    }

    template<class VArray, class TArray>
    inline physx::PxTriangleMesh* ToPxTriangleMesh(physx::PxPhysics* physics, physx::PxCooking* cooking, VArray vertices, TArray triangles) {
        physx::PxTriangleMeshDesc px_mesh_desc;
        px_mesh_desc.setToDefault();
        px_mesh_desc.points.count = static_cast<physx::PxU32>(vertices.size());
        px_mesh_desc.points.stride = sizeof(VArray::value_type);
        px_mesh_desc.points.data = &vertices[0];
        px_mesh_desc.triangles.count = static_cast<physx::PxU32>(triangles.size());
        px_mesh_desc.triangles.stride = sizeof(TArray::value_type);
        px_mesh_desc.triangles.data = &triangles[0];

        physx::PxDefaultMemoryOutputStream write_buffer;
        cooking->cookTriangleMesh(px_mesh_desc, write_buffer);
        physx::PxDefaultMemoryInputData read_buffer(write_buffer.getData(), write_buffer.getSize());
        physx::PxTriangleMesh* triangle_mesh = physics->createTriangleMesh(read_buffer);
        return triangle_mesh;
    }

    //TODO: PxQuat

    inline physx::PxRigidStatic* CreateStatic(
        physx::PxPhysics* physics,
        physx::PxShape* shape,
        const DirectX::SimpleMath::Vector3& position = {0.f, 0.f, 0.f}
    ) {
        physx::PxRigidStatic* rigid_static = physics->createRigidStatic(physx::PxTransform(ToPxVec3(position)));
        rigid_static->attachShape(*shape);
        return rigid_static;
    }

    inline physx::PxRigidDynamic* CreateDynamic(
        physx::PxPhysics* physics,
        physx::PxShape* shape,
        const DirectX::SimpleMath::Vector3& position = {0.f, 0.f, 0.f},
        const DirectX::SimpleMath::Vector3& velocity = { 0.f, 0.f, 0.f },
        float damping = 0.f
    ) {
        physx::PxRigidDynamic* rigid_dynamic = physics->createRigidDynamic(physx::PxTransform(ToPxVec3(position)));
        rigid_dynamic->attachShape(*shape);
        rigid_dynamic->setLinearVelocity(ToPxVec3(velocity));
        //rigid_dynamic->setAngularVelocity(ToPxVec3(velocity));
        //rigid_dynamic->setLinearDamping(damping);
        rigid_dynamic->setAngularDamping(damping);
        return rigid_dynamic;
    }

}

#endif
