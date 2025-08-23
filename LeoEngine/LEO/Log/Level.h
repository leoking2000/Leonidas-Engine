#pragma once
#include <string_view>

namespace LEO::LOG
{
    enum Level
    {
        NONE    = 0,
        FATAL   = 1,
        ERROR   = 2,
        WARN    = 3,
        INFO    = 4,
        DEBUG   = 5,
        VERBOSE = 6
    };

    std::string_view GetLevelName(Level level);
}