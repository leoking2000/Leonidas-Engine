#include "Level.h"

namespace LEO::LOG
{
    std::string_view GetLevelName(Level level)
    {
        switch (level)
        {
            case Level::VERBOSE : return "[Verbose] ";
            case Level::DEBUG   : return "[Debug]   ";
            case Level::INFO    : return "[Info]    ";
            case Level::WARN    : return "[Warning] ";
            case Level::ERROR   : return "[Error]   ";
            case Level::FATAL   : return "[Fatal]   ";
            default             : return "[Unknown] ";
        }
    }
}