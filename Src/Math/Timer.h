#pragma once

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

    TimePoint start = TimePoint();
    TimePoint end   = TimePoint();

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

    static TimePoint GetPoint() noexcept {
        return std::chrono::system_clock::now();
    }
    
    template<class Unit = MS>
    static Time Get() noexcept {
        return std::chrono::duration_cast<Unit>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void Reset() noexcept {
        start = TimePoint();
        end = TimePoint();
    }
};
