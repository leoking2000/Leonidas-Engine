#include "EntryBuilder.h"
#include "Channel.h"
#include <iostream>

namespace leo
{
    EntryBuilder::EntryBuilder(const char* file, const char* functionName, int line)
        :
        Entry{ LogLevel::NONE, "", file, functionName, line}
    {}

    EntryBuilder::~EntryBuilder()
    {
        assert(m_channel != nullptr);

        if (m_channel)
        {
            m_channel->Submit(*this);
        }
    }

    EntryBuilder& EntryBuilder::SetLogLevel(LogLevel l)
    {
        level = l;
        return *this;
    }

    EntryBuilder& EntryBuilder::SetMessage(std::string n)
    {
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::SetChannel(const IChannel* cha)
    {
        m_channel = cha;
        return *this;
    }

    EntryBuilder& EntryBuilder::Verbose(std::string n)
    {
        level = LogLevel::VERBOSE;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Debug(std::string n)
    {
        level = LogLevel::DEBUG;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Info(std::string n)
    {
        level = LogLevel::INFO;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Warn(std::string n)
    {
        level = LogLevel::WARN;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Error(std::string n)
    {
        level = LogLevel::ERROR;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Fatal(std::string n)
    {
        level = LogLevel::FATAL;
        note = std::move(n);
        return *this;
    }
}