#pragma once

#include "Inc/PhysX/PxPhysicsAPI.h"
#include "Inc/PhysX/PxFoundation.h"
#include "Inc/DirectXTK/SimpleMath.h"

namespace physx_helper {

    inline physx::PxVec3 ToPxVec3(const DirectX::SimpleMath::Vector3& vec3) {
        return physx::PxVec3(vec3.x, vec3.y, vec3.z);
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

}
