#pragma once

#include <chrono>

#include "Convert.h"
#include "Timer.h"

struct Time {

    enum class Precision {
        NS,
        US,
        MS,
    };

    Time() noexcept
        : timer(Timer())
        , deltaTime(0)
    {
        timer.Start();
        timer.End();
    }

    double deltaTime;

    void CalcDeltaTime(Precision precision = Precision::MS) {
        timer.End();
        switch (precision) {
        case Precision::NS:
            deltaTime = convert::NSToS<double>(timer.Duration<Timer::NS>());
            break;
        case Precision::US:
            deltaTime = convert::USToS<double>(timer.Duration<Timer::US>());
            break;
        case Precision::MS:
            deltaTime = convert::MSToS<double>(timer.Duration<Timer::MS>());
            break;
        }
        timer.Start();
    }

private:

    Timer timer;

};
