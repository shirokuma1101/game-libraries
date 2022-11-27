/* Math */
#include "TestMath.h"

/* Thread */
#include "TestThread.h"

/* Utility */
#include "TestUtility.h"

/* ExternalDependencies */
#include "TestExternalDependencies.h"


int main(int, char*)
{
    TEST_MATH::TEST_CONSTANT();
    TEST_MATH::TEST_CONVERT();
    TEST_MATH::TEST_DELTATIME();
    TEST_MATH::TEST_EASING();
    TEST_MATH::TEST_MINIMUMMATH();
    TEST_MATH::TEST_PROJECTILEMOTION();
    TEST_MATH::TEST_RANDOM();
    TEST_MATH::TEST_TIMER();

    return 0;
}

/**************************************************
**************************************************/
