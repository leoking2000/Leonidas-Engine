#pragma once
#include "Channel.h"
#include "EntryBuilder.h"
#include "LeoAssert.h"

namespace LEO::LOG
{
    void      Initialization(IChannel* channel = nullptr);
    void      Terminate();
    void      SetDefaultLogChannel(IChannel* channel = nullptr);
    IChannel& GetDefaultLogChannel();
}

#define LEOLOGFATAL(msg, ...)   LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Fatal(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())
#define LEOLOGERROR(msg, ...)   LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Error(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())

#if !PRODUCTION_BUILD
#define LEOLOGWARN(msg, ...)    LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Warn(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())
#define LEOLOGINFO(msg, ...)    LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Info(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())
#define LEOLOGDEBUG(msg, ...)   LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Debug(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())
#define LEOLOGVERBOSE(msg, ...) LEO::LOG::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Verbose(std::format(msg, __VA_ARGS__)).Channel(&LEO::LOG::GetDefaultLogChannel())
#else
#define LEOLOGWARN(msg, ...)
#define LEOLOGINFO(msg, ...)
#define LEOLOGDEBUG(msg, ...)
#define LEOLOGVERBOSE(msg, ...)
#endif





