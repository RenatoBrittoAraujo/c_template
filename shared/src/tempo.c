#include <tempo.h>
#include <time.h>

time_t get_timestamp_now()
{
    return time(0);
}

time_t is_newer(time_t t)
{
    return t > get_timestamp_now();
}
