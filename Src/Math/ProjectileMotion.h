#pragma once

#ifndef GAME_LIBRARIES_MATH_PROJECTILEMOTION_H_
#define GAME_LIBRARIES_MATH_PROJECTILEMOTION_H_

#include <array>
#include <cmath>
#include <tuple>

#include "Math/Constant.h"
#include "Math/Convert.h"

#pragma warning(push)
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union

struct ProjectileMotion {
    union {
        struct {
            float velocity;
            float vectorx;
            float vectory;
            float theta;
            float time;
            float length;
            float height;
            float gravity;
        };
        struct {
            float v0;
            float vx;
            float vy;
            float theta;
            float t;
            float l;
            float h;
            float g;
        };
    };

    ProjectileMotion() noexcept
        : velocity(0.f)
        , vectorx(0.f)
        , vectory(0.f)
        , theta(0.f)
        , time(0.f)
        , length(0.f)
        , height(0.f)
        , gravity(constant::fPI)
    {}
    constexpr ProjectileMotion(float velocity, float vectorx, float vectory, float theta, float time, float length, float height, float gravity = constant::fPI) noexcept
        : velocity(velocity)
        , vectorx(vectorx)
        , vectory(vectory)
        , theta(theta)
        , time(time)
        , length(length)
        , height(height)
        , gravity(gravity)
    {}

    std::tuple<float, float> DisplacementPosition(float _time) const noexcept {
        return { vx * _time, vy * _time - (1.f / 2.f) * g * convert::ToSquare(_time) };
    }

    std::tuple<float, float> DisplacementVector(float _time) const noexcept {
        return { vx, vy - (g * _time) };
    }
};

struct ProjectileMotionFromTimeLength : public ProjectileMotion {
    ProjectileMotionFromTimeLength() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f,  0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromTimeLength(float time, float length, float gravity = constant::fG) noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, time, length, 0.f, gravity)
    {
        v0    = std::sqrt(convert::ToSquare(l / t) + convert::ToSquare(g * t / 2.f));
        theta = std::atan((g * convert::ToSquare(t)) / (2.f * l));
        vx    = v0 * std::cos(theta);
        vy    = v0 * std::sin(theta);
        h     = g * convert::ToSquare(t) / 8.0f; //(1.f / 2.f) * g * convert::ToSquare(t / 2.f);
    }
};

struct ProjectileMotionFromVelocityTheta : public ProjectileMotion {
    ProjectileMotionFromVelocityTheta() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromVelocityTheta(float velocity, float theta, float gravity = constant::fG) noexcept
        : ProjectileMotion(velocity, 0.f, 0.f, theta, 0.f, 0.f, 0.f, gravity)
    {
        vx = v0 * std::cos(theta);
        vy = v0 * std::sin(theta);
        t  = 2.f * (vy / g);
        l  = vx * t;
        h  = convert::ToSquare(vy) / (2.f * g);
    }
};

struct ProjectileMotionFromVelocityLength {
    struct ParabolicMotion {
        bool                              isEnable = false;
        ProjectileMotionFromVelocityTheta projectileMotionFromVelocityTheta;
    };
    std::array<ParabolicMotion, 2> parabolicMotions;

    ProjectileMotionFromVelocityLength() = default;
    ProjectileMotionFromVelocityLength(float velocity, float length, float gravity = constant::fG) noexcept {
        for (int i = 0; i < 2; ++i) {
            parabolicMotions[i].isEnable                          = false;
            parabolicMotions[i].projectileMotionFromVelocityTheta = { velocity, 0.f, gravity };
        }

        if (length == 0.f) {
            //note: 高さを考慮する場合は有効フラグを切り替える
            parabolicMotions[0].projectileMotionFromVelocityTheta.theta = std::atan2(velocity, 0.f);
            parabolicMotions[1].projectileMotionFromVelocityTheta.theta = std::atan2(-velocity, 0.f);
            parabolicMotions[0].isEnable = true;
            parabolicMotions[1].isEnable = true;
        }
        else {
            float a    = (-gravity * convert::ToSquare(length)) / (2.f * convert::ToSquare(velocity));
            float b    = length / a;
            float c    = 1.f; // a / a; // 高さを考慮する場合: (a - pos_vec) / a;
            float root = std::sqrt(-c + convert::ToSquare(b) / 4.f);
            float t    = convert::ToSquare(b) / 4.f - c;

            if (t < 0.f) {
                parabolicMotions[0].isEnable = false;
                parabolicMotions[1].isEnable = false;
            }
            else {
                parabolicMotions[0].projectileMotionFromVelocityTheta.theta = std::atan((-b / 2.f) + root);
                parabolicMotions[1].projectileMotionFromVelocityTheta.theta = std::atan((-b / 2.f) - root);
                parabolicMotions[0].isEnable = true;
                parabolicMotions[1].isEnable = true;
            }
        }
    }

    ProjectileMotionFromVelocityTheta NarrowAngleDisplacement() const {
        return
            NarrowAngleDisplacement(
                [&](int i, float) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta;
                },
                0
            );
    }
    std::tuple<float, float> NarrowAngleDisplacementPosition(float time) const {
        return
            NarrowAngleDisplacement(
                [&](int i, float time) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta.DisplacementPosition(time);
                },
                time
            );
    }
    std::tuple<float, float> NarrowAngleDisplacementVector(float time) const {
        return
            NarrowAngleDisplacement(
                [&](int i, float time) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta.DisplacementVector(time);
                },
                time
            );
    }

    ProjectileMotionFromVelocityTheta WideAngleDisplacement() const {
        return
            WideAngleDisplacement(
                [&](int i, float) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta;
                },
                0
            );
    }
    std::tuple<float, float> WideAngleDisplacementPosition(float time) const {
        return
            WideAngleDisplacement(
                [&](int i, float time) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta.DisplacementPosition(time);
                },
                time
            );
    }
    std::tuple<float, float> WideAngleDisplacementVector(float time) const {
        return
            WideAngleDisplacement(
                [&](int i, float time) {
                    return parabolicMotions[i].projectileMotionFromVelocityTheta.DisplacementVector(time);
                },
                time
            );
    }
    
    bool IsEnable() const noexcept {
        return parabolicMotions[0].isEnable || parabolicMotions[1].isEnable;
    }

private:

    template<class Func>
    auto NarrowAngleDisplacement(Func func, float time) const {
        if (parabolicMotions[0].isEnable && parabolicMotions[1].isEnable) {
            if (parabolicMotions[0].projectileMotionFromVelocityTheta.theta < parabolicMotions[1].projectileMotionFromVelocityTheta.theta) {
                return func(0, time);
            }
            else {
                return func(1, time);
            }
        }
        else if (parabolicMotions[0].isEnable) {
            return func(0, time);
        }
        else if (parabolicMotions[1].isEnable) {
            return func(1, time);
        }
        else {
            return decltype(func(0, 0))();
        }
    }

    template<class Func>
    auto WideAngleDisplacement(Func func, float time) const {
        if (parabolicMotions[0].isEnable && parabolicMotions[1].isEnable) {
            if (parabolicMotions[0].projectileMotionFromVelocityTheta.theta > parabolicMotions[1].projectileMotionFromVelocityTheta.theta) {
                return func(0, time);
            }
            else {
                return func(1, time);
            }
        }
        else if (parabolicMotions[0].isEnable) {
            return func(0, time);
        }
        else if (parabolicMotions[1].isEnable) {
            return func(1, time);
        }
        else {
            return decltype(func(0, 0))();
        }
    }
};

#pragma warning(pop)

#endif
