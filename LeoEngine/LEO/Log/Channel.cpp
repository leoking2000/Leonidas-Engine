#include <iostream>
#include "Channel.h"

namespace leo
{
    void ConsoleChannel::Submit(const Entry& e) const
    {
        if (e.level > m_level) {
            return;
        }

        std::cout << GetLogLevelName(e.level) << e.note;
        std::cout << " (" << e.sourceFunctionName << " " << e.sourceLine << ")" << std::endl;
        //std::cout << "\n" << "(" << e.sourceFile << " " << e.sourceFunctionName << " " << e.sourceLine << ")" << std::endl;
    }

    void ConsoleChannel::SetLoggingLevel(LogLevel level)
    {
        m_level = level;
    }

    void ConsoleChannel::Flush()
    {
        std::cout.flush();
    }
}