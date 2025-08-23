#include <assert.h>
#include "Log.h"

namespace LEO::LOG
{
	static ConsoleChannel g_channel;
	static IChannel* g_user_channel = nullptr;
    static bool g_log_is_initialize = false;

    void Initialization(IChannel* channel)
    {
        assert(g_log_is_initialize == false && "LEO::LOG::Initialization() can't be called when the log is initialized, call LEO::LOG::Terminate()");

        if (channel)
        {
            g_user_channel = channel;
        }

        g_log_is_initialize = true;

        LEOLOGINFO("Log System Initialized!");
    }

    void Terminate()
    {
        assert(g_log_is_initialize == true && "LEO::LOG::Terminate() can't be called when the log is not initialize, call LEO::LOG::Initialization()");

        LEOLOGINFO("Log System Terminate!");

        if (g_user_channel)
        {
            delete g_user_channel;
        }

        g_log_is_initialize = false;     
    }

    void SetDefaultLogChannel(IChannel* channel)
    {
        assert(g_log_is_initialize == true && "LEO::LOG::SetDefaultLogChannel() can't be called when the log is not initialize, call LEO::LOG::Initialization()");

        if (g_user_channel)
        {
            delete g_user_channel;          
        }
        g_user_channel = channel;
    }

    IChannel& GetDefaultLogChannel()
    {
        assert(g_log_is_initialize == true && "LEO::LOG::GetDefaultLogChannel() can't be called when the log is not initialize, call LEO::LOG::Initialization()");

        if (g_user_channel)
        {
            return *g_user_channel;
        }

        return g_channel;
    }

}
