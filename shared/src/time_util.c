#include <time_util.h>

int IGNORE = 0;

void set_time_wait_ignore(int ignore)
{
    IGNORE = ignore; 
}

void wait( int ms )
{
    if (!IGNORE)
    {
        #ifdef _WIN32
            Sleep( ms );
        #else
            usleep( ms*1000 /* to microsseconds*/ );
        #endif
    }
}