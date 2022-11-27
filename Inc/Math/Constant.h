#pragma once

#ifndef GAME_LIBRARIES_MATH_CONSTANT_H_
#define GAME_LIBRARIES_MATH_CONSTANT_H_

namespace constant {

#define CONSTANT_G  9.80665
#define CONSTANT_PI 3.141592653589793 // acos(-1)

    constexpr float  fG  = static_cast<float>(CONSTANT_G);
    constexpr double dG  = static_cast<double>(CONSTANT_G);
    constexpr float  fPI = static_cast<float>(CONSTANT_PI);
    constexpr double dPI = static_cast<double>(CONSTANT_PI);

}

#endif
