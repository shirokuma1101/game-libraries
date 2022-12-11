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
    ProjectileMotion() noexcept
        : velocity(0)
        , vectorx(0)
        , vectory(0)
        , theta(0)
        , time(0)
        , length(0)
        , height(0)
        , gravity(constant::fG)
    {}
    ProjectileMotion(float velocity, float vectorx, float vectory, float theta, float time, float length, float height, float gravity = constant::fG) noexcept
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
        return { vx * _time, vy * _time - convert::ToHalf(g * convert::ToSquare(_time)) };
    }
    std::tuple<float, float> DisplacementVector(float _time) const noexcept {
        return { vx, vy - (g * _time) };
    }

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

struct ProjectileMotionFromHeightLength : public ProjectileMotion {
    ProjectileMotionFromHeightLength() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromHeightLength(float height, float length, float gravity = constant::fG) noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, length, height, gravity)
    {
        theta = std::atan(4.f * h / l);
        v0    = std::sqrt(2.f * g * h) / std::sin(theta);
        vx    = v0 * std::cos(theta);
        vy    = v0 * std::sin(theta);
        t     = 2.f * std::sqrt(2.f * h / g);
    }
};

struct ProjectileMotionFromVelocityTime : public ProjectileMotion {
    ProjectileMotionFromVelocityTime() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromVelocityTime(float velocity, float time, float gravity = constant::fG) noexcept
        : ProjectileMotion(velocity, 0.f, 0.f, 0.f, time, 0.f, 0.f, gravity)
    {
        l     = v0 * t + 0.5f * g * convert::ToSquare(t);
        h     = g * convert::ToSquare(t) / 8.f;
        theta = std::atan(4.f * h / l);
        vx    = v0 * std::cos(theta);
        vy    = v0 * std::sin(theta);
    }
};

struct ProjectileMotionFromTimeLength : public ProjectileMotion {
    ProjectileMotionFromTimeLength() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromTimeLength(float time, float length, float gravity = constant::fG) noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, time, length, 0.f, gravity)
    {
        v0    = std::sqrt(convert::ToSquare(l / t) + convert::ToSquare(g * t / 2.f));
        vx    = v0 * std::cos(theta);
        vy    = v0 * std::sin(theta);
        theta = std::atan((g * convert::ToSquare(t)) / (2.f * l));
        h     = g * convert::ToSquare(t) / 8.0f;
    }
};

struct ProjectileMotionFromThetaLength : public ProjectileMotion {
    ProjectileMotionFromThetaLength() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromThetaLength(float theta, float length, float gravity = constant::fG) noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, theta, 0.f, length, 0.f, gravity)
    {
        v0 = std::sqrt(g * l / (2.f * std::sin(theta) * std::cos(theta)));
        vx = v0 * std::cos(theta);
        vy = v0 * std::sin(theta);
        t  = l / vx;
        h  = g * convert::ToSquare(t) / 8.f;
    }
};

struct ProjectileMotionFromThetaHeight : public ProjectileMotion {
    ProjectileMotionFromThetaHeight() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromThetaHeight(float theta, float height, float gravity = constant::fG) noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, theta, 0.f, 0.f, height, gravity)
    {
        v0 = std::sqrt(2.f * g * h) / std::sin(theta);
        vx = v0 * std::cos(theta);
        vy = v0 * std::sin(theta);
        t  = (2.f * std::sqrt(2.f * g * h)) / g;
        l  = t * vx;
    }
};

struct ProjectileMotionFromVelocityLength : public ProjectileMotion {
    ProjectileMotionFromVelocityLength() noexcept
        : ProjectileMotion(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
    {}
    ProjectileMotionFromVelocityLength(float velocity, float length, float gravity = constant::fG) noexcept
        : ProjectileMotion(velocity, 0.f, 0.f, 0.f, 0.f, length, 0.f, gravity)
    {
        //TODO: 高さを考慮する場合は有効フラグを切り替える
        if (l == 0.f) {
            parabolicMotions[0].isEnable = true;
            parabolicMotions[1].isEnable = true;
            parabolicMotions[0].projectileMotionFromVelocityTheta = ProjectileMotionFromVelocityTheta(v0, 0.f, gravity);
            parabolicMotions[1].projectileMotionFromVelocityTheta = ProjectileMotionFromVelocityTheta(-v0, 0.f, gravity);
        }
        else {
            float a    = (-g * convert::ToSquare(l)) / (2.f * convert::ToSquare(v0));
            float b    = length / a;
            float c    = 1.f; // 高さを考慮する場合: (a - pos_vec) / a;
            float root = std::sqrt(-c + convert::ToSquare(b) / 4.f);
            float d    = convert::ToSquare(b) / 4.f - c;
            if (d < 0.f) {
                parabolicMotions[0].isEnable = false;
                parabolicMotions[1].isEnable = false;
            }
            else {
                parabolicMotions[0].isEnable = true;
                parabolicMotions[1].isEnable = true;
                parabolicMotions[0].projectileMotionFromVelocityTheta = ProjectileMotionFromVelocityTheta(v0, std::atan((-b / 2.f) + root), gravity);
                parabolicMotions[1].projectileMotionFromVelocityTheta = ProjectileMotionFromVelocityTheta(v0, std::atan((-b / 2.f) - root), gravity);
            }
        }
    }

    ProjectileMotionFromVelocityTheta GetNarrowAngle() const noexcept {
        if (parabolicMotions[0].isEnable && parabolicMotions[1].isEnable) {
            return parabolicMotions[0].projectileMotionFromVelocityTheta.theta < parabolicMotions[1].projectileMotionFromVelocityTheta.theta
                ? parabolicMotions[0].projectileMotionFromVelocityTheta
                : parabolicMotions[1].projectileMotionFromVelocityTheta;
        }
        else if (parabolicMotions[0].isEnable) {
            return parabolicMotions[0].projectileMotionFromVelocityTheta;
        }
        else if (parabolicMotions[1].isEnable) {
            return parabolicMotions[1].projectileMotionFromVelocityTheta;
        }
        else {
            return ProjectileMotionFromVelocityTheta();
        }
    }
    ProjectileMotionFromVelocityTheta GetWideAngle() const noexcept {
        if (parabolicMotions[0].isEnable && parabolicMotions[1].isEnable) {
            return parabolicMotions[0].projectileMotionFromVelocityTheta.theta > parabolicMotions[1].projectileMotionFromVelocityTheta.theta
                ? parabolicMotions[0].projectileMotionFromVelocityTheta
                : parabolicMotions[1].projectileMotionFromVelocityTheta;
        }
        else if (parabolicMotions[0].isEnable) {
            return parabolicMotions[0].projectileMotionFromVelocityTheta;
        }
        else if (parabolicMotions[1].isEnable) {
            return parabolicMotions[1].projectileMotionFromVelocityTheta;
        }
        else {
            return ProjectileMotionFromVelocityTheta();
        }
    }

    struct ParabolicMotion {
        bool                              isEnable = false;
        ProjectileMotionFromVelocityTheta projectileMotionFromVelocityTheta;
    };
    std::array<ParabolicMotion, 2> parabolicMotions;
};

#pragma warning(pop)

#endif
