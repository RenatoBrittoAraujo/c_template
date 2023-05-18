#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <shared/inc/threads.h>
#include <shared/inc/shared_util.h>
#include <shared/inc/tcp_ip.h>

#define SHARED_THREADS_ERROR_PARAM_NOT_FOUND 1000
#define SHARED_THREADS_ERROR_NULL_THREAD_START 1001

#define SHM_SIZE 1024

// rules to use this:
// - create a method to pass onto the thread
// - create a ThreadState using create_thread_state
// - call start_thread with the ThreadState

void *thread_procedure(void *ts_args)
{
    ThreadCallParams *params = (ThreadCallParams *)ts_args;
    void *args = params->param;
    ThreadState *ts = (ThreadState *)params->ts;
    if (ts->optional_before != NULL)
    {
        ts->optional_before(params->ts, args);
    }
    if (ts->routine == NULL)
    {
        return handle_error(SHARED_THREADS_ERROR_PARAM_NOT_FOUND,
                           "[shared.threads] ts->routine is not defined");
    }
    ts->routine(params->ts, ts->args);
    if (params->ts->optional_after != NULL)
    {
        ts->optional_after(params->ts, args);
    }
    return NULL;
}

// may set message size to -1 for standard frame size
ThreadState *create_thread_state(size_t message_size)
{
    if (message_size < 0)
    {
        message_size = STANDARD_THREAD_SIZE;
    }
    ThreadState *ts = (ThreadState *)malloc(sizeof(ThreadState));
    ts->__created = 0;
    ts->memory = (ThreadSharedMemory *)malloc(sizeof(ThreadSharedMemory));
    ts->memory->in = (void *)malloc(sizeof(message_size));
    ts->memory->out = (void *)malloc(sizeof(message_size));
    return ts;
}

void start_thread(ThreadState *ts)
{
    if (ts == NULL)
    {
        handle_error(SHARED_THREADS_ERROR_NULL_THREAD_START, "[shared.threads] thread is null");
    }
    if (!ts->__created)
    {
        ts->__created = 1;
    }
    if (!ts->__running)
    {
        ts->__running = 1;
        ThreadCallParams *args = (ThreadCallParams *)malloc(sizeof(ThreadCallParams));
        args->ts = ts;
        args->param = ts->args;
        pthread_t temp_tt;
        pthread_create(&temp_tt, NULL, thread_procedure, args);
        ts->thread_id = temp_tt;
        ts->__running = 0;
    }
}

void wait_thread_end(ThreadState *ts)
{
    pthread_join(ts->thread_id, NULL);
}
