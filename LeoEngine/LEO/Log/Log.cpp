#include <assert.h>
#include "Log.h"

namespace LEO::LOG
{
	static ConsoleChannel g_channel;

    IChannel& GetDefaultLogChannel()
    {
        return g_channel;
    }

}
