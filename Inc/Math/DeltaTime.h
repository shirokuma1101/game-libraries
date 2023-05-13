/**
 * @file DeltaTime.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_DELTATIME_H_
#define GAME_LIBRARIES_MATH_DELTATIME_H_

#include <chrono>

#include "Math/Convert.h"
#include "Math/Timer.h"
#include "Utility/Macro.h"

class DeltaTime
{
public:

    enum class Precision {
        NS,
        US,
        MS,
    };

    DeltaTime() noexcept
        : m_timer(Timer())
        , m_time(0)
    {
        m_timer.Start();
        m_timer.End();
    }

    MACRO_GETTER_AS_IS(double, Get, m_time);

    void Calc(Precision precision = Precision::NS) noexcept {
        m_timer.End();
        switch (precision) {
        case Precision::NS:
            m_time = convert::NSToS(static_cast<double>(m_timer.Duration<Timer::NS>()));
            break;
        case Precision::US:
            m_time = convert::USToS(static_cast<double>(m_timer.Duration<Timer::US>()));
            break;
        case Precision::MS:
            m_time = convert::MSToS(static_cast<double>(m_timer.Duration<Timer::MS>()));
            break;
        }
        m_timer.Start();
    }

private:

    Timer  m_timer;
    double m_time;

};

#endif
