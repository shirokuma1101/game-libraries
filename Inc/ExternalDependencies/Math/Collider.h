#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLIDER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLIDER_H_

#include <list>
#include <memory>

#include "Math/Convert.h"
#include "Utility/Templates.h"
#include "ExternalDependencies/Math/Collision.h"

#include "SimpleMath.h"
#ifndef COLLIDER_DISABLE_LINK_LIBS
#pragma comment(lib, "DirectXTK.lib")
#endif

template<class T, class UnderlyingType = templates::UnderlyingTypeWrapperT<T>>
class Collider
{
public:

    void AddCollisionShape(const std::shared_ptr<ICollisionShape<T>>& shape) {
        if (!shape) return;
        m_spCollisionShapes.push_back(shape);
    }

    bool Intersects(T type, const DirectX::SimpleMath::Matrix& mat, const collision::Ray& target, std::list<collision::Result>* ress) const {
        if (!target.direction.LengthSquared()) {
            return false;
        }

        bool hit = false;

        for (const auto& shape : m_spCollisionShapes) {
            if (!(static_cast<UnderlyingType>(type) & static_cast<UnderlyingType>(shape->GetCollisionType()))) continue;

            collision::Result result;
            if (shape->Intersects(target, mat, &result)) {
                hit = true;
                if (!ress) break;
                ress->push_back(result);
            }
        }

        return hit;
    }
    bool Intersects(T type, const DirectX::SimpleMath::Matrix& mat, const DirectX::BoundingSphere& target, std::list<collision::Result>* ress) const {
        bool hit = false;

        for (const auto& shape : m_spCollisionShapes) {
            if (!(static_cast<UnderlyingType>(type) & static_cast<UnderlyingType>(shape->GetCollisionType()))) continue;
            
            collision::Result result;
            if (shape->Intersects(target, mat, &result)) {
                hit = true;
                if (!ress) break;
                ress->push_back(result);
            }
        }

        return hit;
    }

private:

    std::list<std::shared_ptr<ICollisionShape<T>>> m_spCollisionShapes;

};

#endif
