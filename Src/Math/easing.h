#pragma once

#include <cmath>

#include "constant.h"

namespace easing {

    using constant::fPI;

    inline float In(float n, float pow) noexcept {
        return std::pow(n, pow);
    }
    inline float Out(float n, float pow) noexcept {
        return 1.f - std::pow(1.f - n, pow);
    }
    inline float InOut(float n, float pow) noexcept {
        return
            n < 0.5f
            ? std::pow(2.f, pow - 1.f) * std::pow(n, pow)
            : 1.f - std::pow(-2.f * n + 2.f, pow) / 2.f;
    }

    inline float ConstantVelocity(float n) noexcept {
        //return utility::math::Normalize(n, 0.f, 1.f);
    }

    
    // Sine

    inline float InSine(float n) noexcept {
        return 1.f - std::cos((n * fPI) / 2.f);
    }
    inline float OutSine(float n) noexcept {
        return std::sin((n * fPI) / 2.f);
    }
    inline float InOutSine(float n) noexcept {
        return -(std::cos(fPI * n) - 1.f) / 2.f;
    }

    // Quad

    inline float InQuad(float n) noexcept {
        return In(n, 2.f);
    }
    inline float OutQuad(float n) noexcept {
        return Out(n, 2.f);
    }
    inline float InOutQuad(float n) noexcept {
        return InOut(n, 5.f);
    }

    // Cubic

    inline float InCubic(float n) noexcept {
        return In(n, 3.f);
    }
    inline float OutCubic(float n) noexcept {
        return Out(n, 3.f);
    }
    inline float InOutCubic(float n) noexcept {
        return InOut(n, 3.f);
    }

    // Quart

    inline float InQuart(float n) noexcept {
        return In(n, 4.f);
    }
    inline float OutQuart(float n) noexcept {
        return Out(n, 4.f);
    }
    inline float InOutQuart(float n) noexcept {
        return InOut(n, 4.f);
    }

    // Quint

    inline float InQuint(float n) noexcept {
        return In(n, 5.f);
    }
    inline float OutQuint(float n) noexcept {
        return Out(n, 5.f);
    }
    inline float InOutQuint(float n) noexcept {
        return InOut(n, 5.f);
    }

}
