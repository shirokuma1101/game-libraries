/**
 * @file DeltaTime.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

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
        , time(0)
    {
        timer.Start();
        timer.End();
    }

    void Calc(Precision precision = Precision::NS) {
        timer.End();
        switch (precision) {
        case Precision::NS:
            time = convert::NSToS(static_cast<double>(timer.Duration<Timer::NS>()));
            break;
        case Precision::US:
            time = convert::USToS(static_cast<double>(timer.Duration<Timer::US>()));
            break;
        case Precision::MS:
            time = convert::MSToS(static_cast<double>(timer.Duration<Timer::MS>()));
            break;
        }
        timer.Start();
    }

    double time;

private:

    Timer timer;

};

#endif
