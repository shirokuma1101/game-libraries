/**
 * @file Random.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_RANDOM_H_
#define GAME_LIBRARIES_MATH_RANDOM_H_

#include <random>

namespace random {

    inline int RandomDistribution(int min, int max) {
        std::random_device seed_gen;
        std::mt19937 mt(seed_gen());
        std::uniform_int_distribution<>rand_distribution(min, max);
        return rand_distribution(mt);
    }

    inline double RandomDistribution(double min, double max) {
        std::random_device seed_gen;
        std::mt19937 mt(seed_gen());
        std::uniform_real_distribution<>rand_distribution(min, max);
        return rand_distribution(mt);
    }

}

#endif
