#include <iostream>
#include "Channel.h"

namespace LEO::LOG
{
    void ConsoleChannel::Submit(const Entry& e) const
    {
        if (e.level > m_level) {
            return;
        }

        std::cout << GetLevelName(e.level) << e.note;
        std::cout << " (" << e.sourceFunctionName << " " << e.sourceLine << ")" << std::endl;
        //std::cout << "\n" << "(" << e.sourceFile << " " << e.sourceFunctionName << " " << e.sourceLine << ")" << std::endl;
    }

    void ConsoleChannel::SetLoggingLevel(Level level)
    {
        m_level = level;
    }
}