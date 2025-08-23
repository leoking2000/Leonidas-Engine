#include "EntryBuilder.h"
#include "Channel.h"
#include <iostream>

namespace LEO::LOG
{
    EntryBuilder::EntryBuilder(const char* file, const char* functionName, int line)
        :
        Entry{ Level::NONE, "", file, functionName, line}
    {}

    EntryBuilder::~EntryBuilder()
    {
        if (m_channel)
        {
            m_channel->Submit(*this);
        }
    }

    EntryBuilder& EntryBuilder::LogLevel(Level l)
    {
        level = l;
        return *this;
    }

    EntryBuilder& EntryBuilder::Note(std::string n)
    {
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Channel(const IChannel* cha)
    {
        m_channel = cha;
        return *this;
    }

    EntryBuilder& EntryBuilder::Verbose(std::string n)
    {
        level = Level::VERBOSE;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Debug(std::string n)
    {
        level = Level::DEBUG;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Info(std::string n)
    {
        level = Level::INFO;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Warn(std::string n)
    {
        level = Level::WARN;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Error(std::string n)
    {
        level = Level::ERROR;
        note = std::move(n);
        return *this;
    }

    EntryBuilder& EntryBuilder::Fatal(std::string n)
    {
        level = Level::FATAL;
        note = std::move(n);
        return *this;
    }
}