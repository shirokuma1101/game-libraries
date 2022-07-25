#pragma once

#include <cmath>

#include "Constant.h"

#define NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
#define NAMESPACE_EXTERNAL_END   }
#define NAMESPACE_INTERNAL_BEGIN namespace detail {
#define NAMESPACE_INTERNAL_END   }

namespace easing {

    NAMESPACE_EXTERNAL_BEGIN

    NAMESPACE_INTERNAL_BEGIN
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
    NAMESPACE_INTERNAL_END

    // Sine

    inline float InSine(float n) noexcept {
        return 1.f - std::cos((n * constant::fPI) / 2.f);
    }
    inline float OutSine(float n) noexcept {
        return std::sin((n * constant::fPI) / 2.f);
    }
    inline float InOutSine(float n) noexcept {
        return -(std::cos(constant::fPI * n) - 1.f) / 2.f;
    }

    // Quad

    inline float InQuad(float n) noexcept {
        return detail::In(n, 2.f);
    }
    inline float OutQuad(float n) noexcept {
        return detail::Out(n, 2.f);
    }
    inline float InOutQuad(float n) noexcept {
        return detail::InOut(n, 5.f);
    }

    // Cubic

    inline float InCubic(float n) noexcept {
        return detail::In(n, 3.f);
    }
    inline float OutCubic(float n) noexcept {
        return detail::Out(n, 3.f);
    }
    inline float InOutCubic(float n) noexcept {
        return detail::InOut(n, 3.f);
    }

    // Quart

    inline float InQuart(float n) noexcept {
        return detail::In(n, 4.f);
    }
    inline float OutQuart(float n) noexcept {
        return detail::Out(n, 4.f);
    }
    inline float InOutQuart(float n) noexcept {
        return detail::InOut(n, 4.f);
    }

    // Quint

    inline float InQuint(float n) noexcept {
        return detail::In(n, 5.f);
    }
    inline float OutQuint(float n) noexcept {
        return detail::Out(n, 5.f);
    }
    inline float InOutQuint(float n) noexcept {
        return detail::InOut(n, 5.f);
    }

    NAMESPACE_EXTERNAL_END
}

#undef NAMESPACE_EXTERNAL_BEGIN
#undef NAMESPACE_EXTERNAL_END
#undef NAMESPACE_INTERNAL_BEGIN
#undef NAMESPACE_INTERNAL_END
