#pragma once

#include <chrono>

#include <Math/Constant.h>
#include <Math/Convert.h>
#include <Math/Timer.h>

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

    void CalcDeltaTime(Precision precision = Precision::NS) {
        timer.End();
        switch (precision) {
        case Precision::NS:
            deltaTime = convert::NSToS<double>(static_cast<double>(timer.Duration<Timer::NS>()));
            break;
        case Precision::US:
            deltaTime = convert::USToS<double>(static_cast<double>(timer.Duration<Timer::US>()));
            break;
        case Precision::MS:
            deltaTime = convert::MSToS<double>(static_cast<double>(timer.Duration<Timer::MS>()));
            break;
        }
        timer.Start();
    }

    double deltaTime;

private:

    Timer timer;

};
