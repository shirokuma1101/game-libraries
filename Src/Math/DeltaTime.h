#pragma once

#ifndef GAME_LIBRARIES_MATH_DELTATIME_H_
#define GAME_LIBRARIES_MATH_DELTATIME_H_

#include <chrono>

#include "Math/Constant.h"
#include "Math/Convert.h"
#include "Math/Timer.h"

struct DeltaTime {

    enum class Precision {
        NS,
        US,
        MS,
    };

    DeltaTime() noexcept
        : timer(Timer())
        , deltaTime(0)
    {
        timer.Start();
        timer.End();
    }

    void Calc(Precision precision = Precision::NS) {
        timer.End();
        switch (precision) {
        case Precision::NS:
            deltaTime = convert::NSToS(static_cast<double>(timer.Duration<Timer::NS>()));
            break;
        case Precision::US:
            deltaTime = convert::USToS(static_cast<double>(timer.Duration<Timer::US>()));
            break;
        case Precision::MS:
            deltaTime = convert::MSToS(static_cast<double>(timer.Duration<Timer::MS>()));
            break;
        }
        timer.Start();
    }

    double deltaTime;

private:

    Timer timer;

};

#endif
