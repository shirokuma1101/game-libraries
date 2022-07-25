#pragma once

#include <random>

namespace random {

    inline int RandomDistribution(int min, int max) {
        std::random_device seed_gen;
        std::mt19937 mt(seed_gen());
        std::uniform_int_distribution<>rand_distribution(min, max);
        return rand_distribution(mt);
    }

    inline double RandomDistribution(float min, float max) {
        std::random_device seed_gen;
        std::mt19937 mt(seed_gen());
        std::uniform_real_distribution<>rand_distribution(min, max);
        return rand_distribution(mt);
    }

}
