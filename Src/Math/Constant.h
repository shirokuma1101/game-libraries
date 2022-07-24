#pragma once

namespace constant {

#define CONSTANT_G  9.80665
#define CONSTANT_PI 3.141592653589793 // acos(-1)

    constexpr float fG  = static_cast<float>(CONSTANT_G);
    constexpr float dG  = static_cast<double>(CONSTANT_G);
    constexpr float fPI = static_cast<float>(CONSTANT_PI);
    constexpr float dPI = static_cast<double>(CONSTANT_PI);

}
