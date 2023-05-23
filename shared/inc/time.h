#ifndef SHARED_TIME_H
#define SHARED_TIME_H 1

#ifndef TEST_MODE
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#endif

#include <bits/types/time_t.h>

#define SECOND 1000 // milissegundos

void set_time_wait_ignore(int ignore);

void wait(time_t ms);
time_t get_timestamp_now();
time_t is_newer(time_t t);

#endif