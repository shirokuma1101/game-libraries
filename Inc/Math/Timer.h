/**
 * @file Timer.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_TIMER_H_
#define GAME_LIBRARIES_MATH_TIMER_H_

#include <chrono>

struct Timer {
    using TimePoint = std::chrono::system_clock::time_point;
    using Time      = long long;
    using NS        = std::chrono::nanoseconds;
    using US        = std::chrono::microseconds;
    using MS        = std::chrono::milliseconds;
    using S         = std::chrono::seconds;
    using M         = std::chrono::minutes;
    using H         = std::chrono::hours;

    Timer() noexcept
        : start(TimePoint())
        , end(TimePoint())
    {}
    Timer(const TimePoint& start, const TimePoint& end) noexcept
        : start(start)
        , end(end)
    {}

    void Start() noexcept {
        start = GetPoint();
    }
    void End() noexcept {
        end = GetPoint();
    }

    template<class Unit>
    Time Duration() const noexcept {
        return std::chrono::duration_cast<Unit>(end - start).count();
    }

    void Reset() noexcept {
        start = TimePoint();
        end = TimePoint();
    }

    static TimePoint GetPoint() noexcept {
        return std::chrono::system_clock::now();
    }

    template<class Unit = MS>
    static Time Get() noexcept {
        return std::chrono::duration_cast<Unit>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    TimePoint start;
    TimePoint end;

};

#endif
