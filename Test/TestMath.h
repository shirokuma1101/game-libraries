#pragma once

/* Math */
#include "Math/Constant.h"
#include "Math/Convert.h"
#include "Math/DeltaTime.h"
#include "Math/Easing.h"
#include "Math/MinimumMath.h"
#include "Math/ProjectileMotion.h"
#include "Math/Random.h"
#include "Math/Timer.h"
GAME_LIBRARIES_MATH_CONSTANT_H_
GAME_LIBRARIES_MATH_CONVERT_H_
GAME_LIBRARIES_MATH_DELTATIME_H_
GAME_LIBRARIES_MATH_EASING_H_
GAME_LIBRARIES_MATH_MINIMUMMATH_H_
GAME_LIBRARIES_MATH_PROJECTILEMOTION_H_
GAME_LIBRARIES_MATH_RANDOM_H_
GAME_LIBRARIES_MATH_TIMER_H_

#include "SimpleMath.h"

class TEST_MATH
{
public:
    
    static void TEST_CONSTANT() {
        CONSTANT_G;
        CONSTANT_PI;
        constant::fG;
        constant::fPI;
        constant::dG;
        constant::dPI;
    }

    static void TEST_CONVERT() {
        convert::ToHalf(0.5f);
        convert::ToHalf(10.0);
        convert::ToHalf<float>(0.1);
        convert::ToHalf<double>(0.5f);

        std::vector<float> v = { 10, 11, 12 };
        std::list<float> l = convert::ToContainer<std::list>(v);

        for (const auto& [i, e] : convert::Enumerate(v)) {
            //assert::ShowWarning("i: " + std::to_string(i) + ", e: " + std::to_string(e));
        }
        for (const auto& [i, e] : convert::Enumerate(l)) {
            //assert::ShowWarning("i: " + std::to_string(i) + ", e: " + std::to_string(e));
        }
    }

    static void TEST_DELTATIME() {

    }

    static void TEST_EASING() {
        
    }

    static void TEST_MINIMUMMATH() {
        #define VECTOR2_COMPARE(lhs, ope, rhs) (lhs).x ope (rhs).x && (lhs).y ope (rhs).y
        DirectX::SimpleMath::Vector2 DxVec2(1, 2);
        minimum_math::Vector2        MinVec2(1, 2);

        assert(VECTOR2_COMPARE(DxVec2,                 ==, MinVec2));
        assert(VECTOR2_COMPARE(-DxVec2,                ==, -MinVec2));
        
        assert(VECTOR2_COMPARE(DxVec2 + DxVec2,        ==, MinVec2 + MinVec2));
        assert(VECTOR2_COMPARE(DxVec2 - DxVec2,        ==, MinVec2 - MinVec2));
        assert(VECTOR2_COMPARE(DxVec2 * DxVec2,        ==, MinVec2 * MinVec2));
        assert(VECTOR2_COMPARE(DxVec2 / DxVec2,        ==, MinVec2 / MinVec2));

        assert(                DxVec2.Dot({ 1, 2 })    ==  MinVec2.Dot({ 1, 2 }));
        assert(VECTOR2_COMPARE(DxVec2.Cross({ 1, 2 }), ==, MinVec2.Cross({ 1, 2 })));
        
        assert(                DxVec2.Length()         ==  MinVec2.Length());
        assert(                DxVec2.LengthSquared()  ==  MinVec2.LengthSquared());


        #define VECTOR3_COMPARE(lhs, ope, rhs) (lhs).x ope (rhs).x && (lhs).y ope (rhs).y && (lhs).z ope (rhs).z
        DirectX::SimpleMath::Vector3 DxVec3(1, 2, 3);
        minimum_math::Vector3        MinVec3(1, 2, 3);

        assert(VECTOR3_COMPARE(DxVec3,                    ==, MinVec3));
        assert(VECTOR3_COMPARE(-DxVec3,                   ==, -MinVec3));
        
        assert(VECTOR3_COMPARE(DxVec3 + DxVec3,           ==, MinVec3 + MinVec3));
        assert(VECTOR3_COMPARE(DxVec3 - DxVec3,           ==, MinVec3 - MinVec3));
        assert(VECTOR3_COMPARE(DxVec3 * DxVec3,           ==, MinVec3 * MinVec3));
        assert(VECTOR3_COMPARE(DxVec3 / DxVec3,           ==, MinVec3 / MinVec3));

        assert(                DxVec3.Dot({ 1, 2, 3 })    ==  MinVec3.Dot({ 1, 2, 3 }));
        assert(VECTOR3_COMPARE(DxVec3.Cross({ 1, 2, 3 }), ==, MinVec3.Cross({ 1, 2, 3 })));
        
        assert(                DxVec3.Length()            ==  MinVec3.Length());
        assert(                DxVec3.LengthSquared()     ==  MinVec3.LengthSquared());


        #define MATRIX4x4_COMPARE(lhs, ope, rhs) (lhs).m[0][0] ope (rhs).m4x4[0][0] && (lhs).m[0][1] ope (rhs).m4x4[0][1] && (lhs).m[0][2] ope (rhs).m4x4[0][2] && (lhs).m[0][3] ope (rhs).m4x4[0][3] && \
                                                 (lhs).m[1][0] ope (rhs).m4x4[1][0] && (lhs).m[1][1] ope (rhs).m4x4[1][1] && (lhs).m[1][2] ope (rhs).m4x4[1][2] && (lhs).m[1][3] ope (rhs).m4x4[1][3] && \
                                                 (lhs).m[2][0] ope (rhs).m4x4[2][0] && (lhs).m[2][1] ope (rhs).m4x4[2][1] && (lhs).m[2][2] ope (rhs).m4x4[2][2] && (lhs).m[2][3] ope (rhs).m4x4[2][3] && \
                                                 (lhs).m[3][0] ope (rhs).m4x4[3][0] && (lhs).m[3][1] ope (rhs).m4x4[3][1] && (lhs).m[3][2] ope (rhs).m4x4[3][2] && (lhs).m[3][3] ope (rhs).m4x4[3][3]
        DirectX::SimpleMath::Matrix DxMat;
        minimum_math::Matrix4x4     MinMat;
    }

    static void TEST_PROJECTILEMOTION() {

    }

    static void TEST_RANDOM() {

    }

    static void TEST_TIMER() {

    }
    
};
