#pragma once
#include "Channel.h"
#include "EntryBuilder.h"
#include "LeoAssert.h"

namespace LEO
{
    IChannel& GetDefaultLogChannel();
}

#define LEOLOGFATAL(msg, ...)   LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Fatal(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())
#define LEOLOGERROR(msg, ...)   LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Error(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())
#define LEOLOGWARN(msg, ...)    LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Warn(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())

#if !PRODUCTION_BUILD
#define LEOLOGINFO(msg, ...)    LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Info(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())
#define LEOLOGDEBUG(msg, ...)   LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Debug(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())
#define LEOLOGVERBOSE(msg, ...) LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Verbose(std::format(msg, ##__VA_ARGS__)).SetChannel(&LEO::GetDefaultLogChannel())
#else
#define LEOLOGWARN(msg, ...)
#define LEOLOGINFO(msg, ...)
#define LEOLOGDEBUG(msg, ...)
#define LEOLOGVERBOSE(msg, ...)
#endif





