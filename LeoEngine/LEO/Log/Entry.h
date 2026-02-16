#pragma once
#include <string>
#include "LogLevel.h"

namespace leo
{
    struct Entry
    {
        LogLevel level = LogLevel::NONE;
        std::string note = "";
        const char* sourceFile = "";
        const char* sourceFunctionName = "";
        i32 sourceLine = 0;
    };
}