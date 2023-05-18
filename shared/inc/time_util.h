#ifndef TIME_UTIL
#define TIME_UTIL 1

#ifndef TEST_MODE
    #ifdef _WIN32
        #include <windows.h>
    #else
        #include <unistd.h>
    #endif
#endif

void wait( int ms );
void set_time_wait_ignore( int ignore );

#endif