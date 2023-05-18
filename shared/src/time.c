#include <shared/inc/time.h>
#include <time.h>

void set_time_wait_ignore(int ignore_time_wait)
{
    IGNORE_TIME_WAIT = ignore_time_wait;
}

time_t get_timestamp_now()
{
    return time(0);
}

void wait(time_t ms)
{
    if (!IGNORE_TIME_WAIT)
    {
#ifdef _WIN32
        Sleep(ms);
#else
        usleep(ms * SECOND /* to microsseconds*/);
#endif
    }
}

time_t is_newer(time_t t)
{
    return t > get_timestamp_now();
}
