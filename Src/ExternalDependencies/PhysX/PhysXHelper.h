#pragma once

#include "SimpleMath.h"
#include "PxPhysicsAPI.h"
#include "PxFoundation.h"

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

    template<class Array>
    inline physx::PxTriangleMesh* ToPxTriangleMesh(physx::PxPhysics* physics, physx::PxCooking* cooking, Array* vertices, Array* triangles) {
        physx::PxTriangleMeshDesc px_mesh_desc;
        px_mesh_desc.setToDefault();
        px_mesh_desc.points.count = vertices.size();
        px_mesh_desc.points.stride = sizeof(Array::value_type);
        px_mesh_desc.points.data = vertices[0];
        px_mesh_desc.triangles.count = triangles.size();
        px_mesh_desc.triangles.stride = sizeof(Array::value_type);
        px_mesh_desc.triangles.data = triangles[0];

        physx::PxDefaultMemoryOutputStream write_buffer;
        cooking->cookTriangleMesh(px_mesh_desc, write_buffer);
        physx::PxDefaultMemoryInputData read_buffer(write_buffer.getData(), write_buffer.getSize());
        physx::PxTriangleMesh* triangle_mesh = physics->createTriangleMesh(read_buffer);
        return triangle_mesh;
    }

    inline physx::PxRigidStatic* CreateStatic(
        physx::PxPhysics* physics,
        physx::PxShape* shape,
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Quaternion& rotation
    ) {
        physx::PxRigidStatic* rigid_static = physics->createRigidStatic(physx::PxTransform(ToPxVec3(position), ToPxQuat(rotation)));
        rigid_static->attachShape(*shape);
        return rigid_static;
    }

    inline physx::PxRigidDynamic* CreateDynamic(
        physx::PxPhysics* physics,
        physx::PxShape* shape,
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Quaternion& rotation,
        const DirectX::SimpleMath::Vector3& velocity = { 0.f, 0.f, 0.f },
        float damping = 0.f
    ) {
        physx::PxRigidDynamic* rigid_dynamic = physics->createRigidDynamic(physx::PxTransform(ToPxVec3(position), ToPxQuat(rotation)));
        rigid_dynamic->attachShape(*shape);
        rigid_dynamic->setLinearVelocity(ToPxVec3(velocity));
        rigid_dynamic->setAngularDamping(damping);
        return rigid_dynamic;
    }

    /* Geometry manual */
    // URL:"https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html"

    struct Geometry {
        Geometry(physx::PxPhysics* physics, physx::PxMaterial* material)
            : physics(physics)
            , material(material)
            , shape(nullptr)
        {}

        virtual physx::PxShape* Make() = 0;
        
        physx::PxPhysics*  physics;
        physx::PxMaterial* material;
        physx::PxShape*    shape;
    };

    struct Sphere : public Geometry {
        Sphere(physx::PxPhysics* physics, physx::PxMaterial* material, float radius)
            : Geometry(physics, material)
            , radius(radius)
        {
            Make();
        }

        physx::PxShape* Make() override {
            return shape = physics->createShape(physx::PxSphereGeometry(radius), *material);
        }

        float radius;
    };

    struct Cupsule : public Geometry {
        Cupsule(physx::PxPhysics* physics, physx::PxMaterial* material, float radius, float half_height)
            : Geometry(physics, material)
            , radius(radius)
            , halfHeight(half_height)
        {
            Make();
        }

        physx::PxShape* Make() override {
            return shape = physics->createShape(physx::PxCapsuleGeometry(radius, halfHeight), *material);
        }

        float radius;
        float halfHeight;
    };

    struct Box : public Geometry {
        Box(physx::PxPhysics* physics, physx::PxMaterial* material, const DirectX::SimpleMath::Vector3& half_extent)
            : Geometry(physics, material)
            , halfExtent(half_extent)
        {
            Make();
        }

        physx::PxShape* Make() override {
            return shape = physics->createShape(physx::PxBoxGeometry(ToPxVec3(halfExtent)), *material);
        }

        DirectX::SimpleMath::Vector3 halfExtent;
    };

    struct Plane : public Geometry {
        Plane(physx::PxPhysics* physics, physx::PxMaterial* material)
            : Geometry(physics, material)
        {
            Make();
        }

        physx::PxShape* Make() override {
            return shape = physics->createShape(physx::PxPlaneGeometry(), *material);
        }

    };

}
