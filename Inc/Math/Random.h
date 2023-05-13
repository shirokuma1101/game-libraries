/**
 * @file Random.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_RANDOM_H_
#define GAME_LIBRARIES_MATH_RANDOM_H_

#include <random>

/**
 * @namespace random
 * @brief Random distribution
 */
namespace random {

    /**
     * @brief Generate a random integer between min and max (inclusive)
     * @param min The minimum value that the random number can be
     * @param max The maximum value that the random number can be
     * @return An integer between min and max (inclusive)
     */
    inline int RandomDistribution(int min, int max) {
        // Seed the random number generator with a non-deterministic seed
        std::random_device seed_gen;
        // Create the Mersenne Twister pseudo-random number generator
        std::mt19937 mt(seed_gen());
        // Create a uniform distribution of integers between min and max
        std::uniform_int_distribution<> rand_distribution(min, max);
        // Generate a random integer between min and max (inclusive)
        return rand_distribution(mt);
    }

    /**
     * @brief Generate a random floating point number between min and max (inclusive)
     * @param min The minimum value that the random number can be
     * @param max The maximum value that the random number can be
     * @return A floating point number between min and max (inclusive)
     */
    inline double RandomDistribution(double min, double max) {
        std::random_device seed_gen;
        std::mt19937 mt(seed_gen());
        std::uniform_real_distribution<>rand_distribution(min, max);
        return rand_distribution(mt);
    }

}

#endif
