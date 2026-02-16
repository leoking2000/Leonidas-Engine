#pragma once
#include "Channel.h"
#include "EntryBuilder.h"
#include "LeoAssert.h"

namespace leo
{
    IChannel& GetDefaultLogChannel();
}

#define LEOLOGFATAL(msg, ...)   leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Fatal(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())
#define LEOLOGERROR(msg, ...)   leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Error(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())
#define LEOLOGWARN(msg, ...)    leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Warn(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())

#if !PRODUCTION_BUILD
#define LEOLOGINFO(msg, ...)    leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Info(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())
#define LEOLOGDEBUG(msg, ...)   leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Debug(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())
#define LEOLOGVERBOSE(msg, ...) leo::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Verbose(std::format(msg, ##__VA_ARGS__)).SetChannel(&leo::GetDefaultLogChannel())
#else
#define LEOLOGWARN(msg, ...)
#define LEOLOGINFO(msg, ...)
#define LEOLOGDEBUG(msg, ...)
#define LEOLOGVERBOSE(msg, ...)
#endif





