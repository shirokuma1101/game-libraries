#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLISION_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLISION_H_

#include "SimpleMath.h"
#ifndef COLLISION_DISABLE_LINK_LIBS
#pragma comment(lib, "DirectXTK.lib")
#endif

namespace collision {
    struct Ray : public DirectX::SimpleMath::Ray {
        Ray() noexcept
            : DirectX::SimpleMath::Ray()
            , range(0.f)
        {}
        Ray(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction, float range)
            : DirectX::SimpleMath::Ray(position, direction)
            , range(range)
        {}

        float range = 0.f;
    };

    using BoundingSphere = DirectX::BoundingSphere;

    struct Result {
        bool                         overlap = false;
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 direction;
        float                        depth = 0.f;
    };

    template<class Container>
    inline Result GetNearest(const Container& ress) {
        Result res;
        if (!ress.size()) return res;
        res = *ress.begin();
        for (const auto& e : ress) {
            if (e.depth < res.depth) {
                res = e;
            }
        }
        return res;
    }

    namespace mesh {
        inline void PointToTriangle(
            const DirectX::SimpleMath::Vector3& p,
            const DirectX::SimpleMath::Vector3& a,
            const DirectX::SimpleMath::Vector3& b,
            const DirectX::SimpleMath::Vector3& c,
            DirectX::SimpleMath::Vector3* res = nullptr
        ) {
            DirectX::SimpleMath::Vector3 ab = b - a;
            DirectX::SimpleMath::Vector3 ac = c - a;
            DirectX::SimpleMath::Vector3 ap = p - a;
            float d1 = ab.Dot(ap);
            float d2 = ac.Dot(ap);

            if (d1 <= 0.f && d2 <= 0.f) {
                *res = a; // 1,0,0
                return;
            }


            DirectX::SimpleMath::Vector3 bp = p - b;
            float d3 = ab.Dot(bp);
            float d4 = ac.Dot(bp);

            if (d3 >= 0.f && d4 <= d3) {
                *res = b; // 0,1,0
                return;
            }


            float vc = d1 * d4 - d3 * d2;

            if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
                float v = d1 / (d1 - d3);
                *res = a + ab * v; // 1-v,v,0
                return;
            }


            DirectX::SimpleMath::Vector3 cp = p - c;
            float d5 = ab.Dot(cp);
            float d6 = ac.Dot(cp);

            if (d6 >= 0.f && d5 <= d6) {
                *res = c; // 0,0,1
                return;
            }


            float vb = d5 * d2 - d1 * d6;

            if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
                float w = d2 / (d2 - d6);
                *res = a + ac * w; // 1-w,0,w
                return;
            }


            float va = d3 * d6 - d5 * d4;

            if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
                float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
                *res = b + (c - b) * w; // 0,1-w,w
                return;
            }


            float denom = 1.f / (va + vb + vc);
            float v = vb * denom;
            float w = vc * denom;
            *res = a + ab * v + ac * w;
        }
    }
}

template<class T>
class ICollisionShape
{
public:

    ICollisionShape(T type)
        : m_collisitonType(type)
    {}
    virtual ~ICollisionShape() {}

    virtual T GetCollisionType() const noexcept final {
        return m_collisitonType;
    }
    virtual bool Intersects(const collision::Ray&            target, const DirectX::SimpleMath::Matrix& mat, collision::Result* res) const = 0;
    virtual bool Intersects(const collision::BoundingSphere& target, const DirectX::SimpleMath::Matrix& mat, collision::Result* res) const = 0;

private:

    T m_collisitonType;

};

template<class T>
class CollisionSphere : public ICollisionShape<T>
{
public:

    CollisionSphere(T type, const DirectX::BoundingSphere& bounding_sphere)
        : ICollisionShape<T>(type)
        , m_boundingSphere(bounding_sphere)
    {}
    ~CollisionSphere() override {}

    virtual bool Intersects(const collision::Ray& target, const DirectX::SimpleMath::Matrix& mat, collision::Result* res = nullptr) const override {
        memset(res, 0, sizeof(collision::Result));
        collision::BoundingSphere bounding_sphere;
        float distance = 0.f;

        m_boundingSphere.Transform(bounding_sphere, mat);
        bool hit = m_boundingSphere.Intersects(target.position, target.direction, distance);
        hit &= (target.range >= distance);

        if (!res) {
            return hit;
        }

        res->overlap = hit;
        if (hit) {
            res->position  = target.position + (target.direction * distance);
            res->direction = target.direction * (-1);
            res->depth     = target.range - distance;
        }

        return hit;
    }
    virtual bool Intersects(const collision::BoundingSphere& target, const DirectX::SimpleMath::Matrix& mat, collision::Result* res = nullptr) const override {
        memset(res, 0, sizeof(collision::Result));
        collision::BoundingSphere bounding_sphere;

        m_boundingSphere.Transform(bounding_sphere, mat);
        bool hit = m_boundingSphere.Intersects(target);

        if (!res) {
            return hit;
        }

        res->overlap = hit;
        if (hit) {
            DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3(target.Center) - DirectX::SimpleMath::Vector3(bounding_sphere.Center);

            res->direction = dir;
            res->direction.Normalize();
            res->depth     = (target.Radius + m_boundingSphere.Radius) - dir.Length();
            res->position  = m_boundingSphere.Center + (res->direction * (m_boundingSphere.Radius + convert::ToHalf(res->depth)));
        }

        return hit;
    }

private:

    collision::BoundingSphere m_boundingSphere;

};

#endif
