#pragma once

#include <random>

namespace random {

    inline int RandomDistribution(int min, int max) {
        std::mt19937 mt(std::random_device());
        std::uniform_int_distribution<> rand_distribution(min, max);
        return rand_distribution(mt);
    }

    inline float RandomDistribution(float min, float max) {
        std::mt19937 mt(std::random_device());
        std::uniform_real_distribution<> rand_distribution(min, max);
        return rand_distribution(mt);
    }

}
