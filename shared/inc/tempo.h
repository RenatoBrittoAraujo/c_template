#ifndef TIMESTAMP_FUNC
#define TIMESTAMP_FUNC 1

#ifndef TEST_MODE
#include <time.h>
#else
typedef long int time_t;
time_t time(time_t *__timer)
{
    return 0;
}
#endif

#define SEGUNDO 1000 // milissegundos

time_t get_timestamp_now();
time_t is_newer(time_t t);

#endif