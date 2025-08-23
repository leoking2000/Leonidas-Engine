#pragma once
#include <string>
#include "Level.h"

namespace LEO::LOG
{
    struct Entry
    {
        Level level = Level::NONE;
        std::string note = "";
        const char* sourceFile = "";
        const char* sourceFunctionName = "";
        int sourceLine = 0;
    };
}