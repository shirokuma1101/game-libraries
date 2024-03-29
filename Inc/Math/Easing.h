﻿/**
 * @file Easing.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_EASING_H_
#define GAME_LIBRARIES_MATH_EASING_H_

#include <cmath>

#include "Math/Constant.h"
#include "Utility/Macro.h"

/**
 * @namespace easing
 * @brief Easing functions
 */
namespace easing {

    namespace Sine {
        inline float In(float n) noexcept {
            return 1.f - std::cos((n * constant::fPI) / 2.f);
        }
        inline float Out(float n) noexcept {
            return std::sin((n * constant::fPI) / 2.f);
        }
        inline float InOut(float n) noexcept {
            return -(std::cos(constant::fPI * n) - 1.f) / 2.f;
        }
    }

    MACRO_NAMESPACE_EXTERNAL_BEGIN
    MACRO_NAMESPACE_INTERNAL_BEGIN
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
    MACRO_NAMESPACE_INTERNAL_END

    namespace Quad {
        inline float In(float n) noexcept {
            return detail::In(n, 2.f);
        }
        inline float Out(float n) noexcept {
            return detail::Out(n, 2.f);
        }
        inline float InOut(float n) noexcept {
            return detail::InOut(n, 2.f);
        }
    }

    namespace Cubic {
        inline float In(float n) noexcept {
            return detail::In(n, 3.f);
        }
        inline float Out(float n) noexcept {
            return detail::Out(n, 3.f);
        }
        inline float InOut(float n) noexcept {
            return detail::InOut(n, 3.f);
        }
    }

    namespace Quart {
        inline float In(float n) noexcept {
            return detail::In(n, 4.f);
        }
        inline float Out(float n) noexcept {
            return detail::Out(n, 4.f);
        }
        inline float InOut(float n) noexcept {
            return detail::InOut(n, 4.f);
        }
    }

    namespace Quint {
        inline float In(float n) noexcept {
            return detail::In(n, 5.f);
        }
        inline float Out(float n) noexcept {
            return detail::Out(n, 5.f);
        }
        inline float InOut(float n) noexcept {
            return detail::InOut(n, 5.f);
        }
    }

    namespace Power1 = Quad;
    namespace Power2 = Cubic;
    namespace Power3 = Quart;
    namespace Power4 = Quint;

    template<class Func>
    inline float NormalizedEasing(Func func, float x, float x_start, float x_end, int start_displacement, int end_displacement) {
        return convert::Normalize(
            func(convert::Normalize(x, x_start, x_end)),
            0.f,
            1.f,
            start_displacement,
            end_displacement
        );
    }

    MACRO_NAMESPACE_EXTERNAL_END
}

#endif
