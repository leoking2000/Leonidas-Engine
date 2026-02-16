#include <assert.h>
#include "Log.h"

namespace leo
{
	static ConsoleChannel g_channel;

    IChannel& GetDefaultLogChannel()
    {
        return g_channel;
    }

}
