#pragma once
#include "Entry.h"

namespace LEO
{
    class IChannel
    {
    public:
        virtual void Submit(const Entry& e) const    = 0;
        virtual void SetLoggingLevel(LogLevel level) = 0;
        virtual void Flush() {}

        virtual ~IChannel() { Flush(); }
    };

    class ConsoleChannel : public IChannel
    {
    public:
        virtual void Submit(const Entry& e) const override;
        virtual void SetLoggingLevel(LogLevel level) override;
        virtual void Flush() override;

        ~ConsoleChannel() {};
    private:
        LogLevel m_level = LogLevel::INFO;
    };
}