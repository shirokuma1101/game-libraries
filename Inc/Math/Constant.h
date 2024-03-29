﻿/**
 * @file Constant.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_CONSTANT_H_
#define GAME_LIBRARIES_MATH_CONSTANT_H_

/**
 * @namespace constant
 * @brief Namespace for constants used throughout the program.
 */
namespace constant {

    /**
     * @brief Macro defining the acceleration due to gravity (in m/s^2).
     */
    #define CONSTANT_G  9.80665
    /**
     * @brief Macro defining the value of pi (approximately 3.14159).
     */
    #define CONSTANT_PI 3.141592653589793 // acos(-1)

    /**
     * @brief Constant value of acceleration due to gravity, as a float.
     */
    constexpr float  fG  = static_cast<float>(CONSTANT_G);
    /**
     * @brief Constant value of acceleration due to gravity, as a double.
     */
    constexpr double dG  = static_cast<double>(CONSTANT_G);
    /**
     * @brief Constant value of pi, as a float.
     */
    constexpr float  fPI = static_cast<float>(CONSTANT_PI);
    /**
     * @brief Constant value of pi, as a double.
     */
    constexpr double dPI = static_cast<double>(CONSTANT_PI);

}

#endif
