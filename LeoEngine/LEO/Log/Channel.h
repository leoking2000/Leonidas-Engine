#pragma once
#include "Entry.h"

namespace LEO::LOG
{
    class IChannel
    {
    public:
        virtual void Submit(const Entry& e) const = 0;
        virtual void SetLoggingLevel(Level level) = 0;

        virtual ~IChannel() = default;
    };

    class ConsoleChannel : public IChannel
    {
    public:
        void Submit(const Entry& e) const override;
        virtual void SetLoggingLevel(Level level);

        ~ConsoleChannel() {};
    private:
        Level m_level = Level::INFO;
    };
}