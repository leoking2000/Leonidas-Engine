#pragma once
#include <string_view>

namespace LEO
{
    enum LogLevel : u8
    {
        NONE    = 0u,
        FATAL   = 1u,
        ERROR   = 2u,
        WARN    = 3u,
        INFO    = 4u,
        DEBUG   = 5u,
        VERBOSE = 6u
    };

    std::string_view GetLogLevelName(LogLevel level);
}