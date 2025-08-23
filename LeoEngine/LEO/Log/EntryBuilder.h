#pragma once
#include "Entry.h"

namespace LEO::LOG
{
    class IChannel;

    class EntryBuilder : private Entry
    {
    public:
        EntryBuilder(const char* file, const char* functionName, int line);
        ~EntryBuilder();

        EntryBuilder& LogLevel(Level l);
        EntryBuilder& Note(std::string n = "");
        EntryBuilder& Channel(const IChannel* cha);

        EntryBuilder& Verbose(std::string n = "");
        EntryBuilder& Debug(std::string n = "");
        EntryBuilder& Info(std::string n = "");
        EntryBuilder& Warn(std::string n = "");
        EntryBuilder& Error(std::string n = "");
        EntryBuilder& Fatal(std::string n = "");
    private:
        const IChannel* m_channel = nullptr;
    };
}