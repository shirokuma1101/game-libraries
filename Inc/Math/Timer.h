/**
 * @file Timer.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_TIMER_H_
#define GAME_LIBRARIES_MATH_TIMER_H_

#include <chrono>

class Timer
{
public:

    using TimePoint = std::chrono::system_clock::time_point;
    using Time      = long long;
    using NS        = std::chrono::nanoseconds;
    using US        = std::chrono::microseconds;
    using MS        = std::chrono::milliseconds;
    using S         = std::chrono::seconds;
    using M         = std::chrono::minutes;
    using H         = std::chrono::hours;

    Timer() noexcept
        : m_start(TimePoint())
        , m_end(TimePoint())
    {}
    Timer(const TimePoint& start, const TimePoint& end) noexcept
        : m_start(start)
        , m_end(end)
    {}

    void Start() noexcept {
        m_start = GetPoint();
    }
    void End() noexcept {
        m_end = GetPoint();
    }

    template<class Unit>
    Time Duration() const noexcept {
        return std::chrono::duration_cast<Unit>(m_end - m_start).count();
    }

    void Reset() noexcept {
        m_start = TimePoint();
        m_end   = TimePoint();
    }

    static TimePoint GetPoint() noexcept {
        return std::chrono::system_clock::now();
    }

    template<class Unit = MS>
    static Time Get() noexcept {
        return std::chrono::duration_cast<Unit>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

private:

    TimePoint m_start;
    TimePoint m_end;

};

#endif
