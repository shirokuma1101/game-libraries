#pragma once

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "Utility/Memory.h"
#include "Utility/StdC++.h"
#include "Utility/Templates.h"
GAME_LIBRARIES_UTILITY_ASSERT_H_
GAME_LIBRARIES_UTILITY_MACRO_H_
GAME_LIBRARIES_UTILITY_MEMORY_H_
GAME_LIBRARIES_UTILITY_TEMPLATES_H_

class TEST_UTILITY
{
public:

    static void TEST_ASSERT() {
        assert::ShowInfo(ASSERT_FILE_LINE, "information");
        assert::ShowInfo("information");
        assert::ShowWarning(ASSERT_FILE_LINE, "warning");
        assert::ShowWarning("warning");
        assert::ShowError(ASSERT_FILE_LINE, "error");
        assert::ShowError("error");
        assert::ExceptionThrow("exception");
    }

    static void TEST_MACRO() {
        
    }

    static void TEST_MEMORY() {
        
    }

    static void TEST_STDCPP() {
        
    }

    static void TEST_TEMPLATES() {
        
    }
    
};
