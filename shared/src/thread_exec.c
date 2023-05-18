#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <thread_exec.h>
#include <shared_util.h>
#include <comunicacao.h>

#define SHM_SIZE 1024

// rules to use this:
// - create a method to pass onto the thread
// - create a ThreadState using create_thread_state
// - call start_thread with the ThreadState

void *thread_procedure(void *ts_args)
{
    log_print("[THREAD_EXEC] thread_procedure(1)\n", LEVEL_DEBUG);

    printf("ts_args = %p\n", ts_args);
    fflush(NULL);
    ThreadCallParams *params = (ThreadCallParams *)ts_args;
    log_print("[THREAD_EXEC] thread_procedure(2)\n", LEVEL_DEBUG);
    void *args = params->param;
    printf("args = %p\n", args);

    log_print("[THREAD_EXEC] thread_procedure(3)\n", LEVEL_DEBUG);
    ThreadState *ts = (ThreadState *)params->ts;
    printf("ts pointer = %p\n", ts);
    fflush(NULL);
    printf("t_id = %lu\n", ts->thread_id);
    fflush(NULL);
    log_print("[THREAD_EXEC] thread_procedure(4)\n", LEVEL_DEBUG);
    printf("ts = %p ts_id = %lu\n", ts, ts->thread_id);
    fflush(NULL);

    // printf("ts->optional_before = %p\n", ts->optional_before);
    // fflush(NULL);
    // if (ts->optional_before != NULL)
    // {
    //     log_print("[THREAD_EXEC] thread_procedure(5)\n", LEVEL_DEBUG);
    //     ts->optional_before(params->ts, args);
    // }

    log_print("[THREAD_EXEC] CADE O TS->ROUTINE()?\n", LEVEL_DEBUG);
    printf("[TEXEC] %p %p \n", ts_args, ts->routine);
    fflush(NULL);
    printf("[TEXEC] ts = %p\n", params->ts);
    fflush(NULL);
    printf("[TEXEC] estacionamento = %p\n", ts->args);
    fflush(NULL);
    log_print("[THREAD_EXEC] 2 CADE O TS->ROUTINE()?\n", LEVEL_DEBUG);

    ts->routine(params->ts, ts->args);
    log_print("[THREAD_EXEC] thread_procedure(6)\n", LEVEL_DEBUG);
    // if (params->ts->optional_after != NULL)
    // {
    //     log_print("[THREAD_EXEC] thread_procedure(7)\n", LEVEL_DEBUG);
    //     ts->optional_after(params->ts, args);
    // }
    log_print("[THREAD_EXEC] thread_procedure(8)\n", LEVEL_DEBUG);
    return NULL;
}

ThreadState *create_thread_state()
{
    log_print("[THREAD_EXEC] create_thread_state()\n", LEVEL_DEBUG);

    ThreadState *ts = (ThreadState *)malloc(sizeof(ThreadState));
    ts->__created = 0;
    ts->memory = (ThreadSharedMemory *)malloc(sizeof(ThreadSharedMemory));
    ts->memory->in = (void *)malloc(sizeof(EstadoEstacionamento));
    ts->memory->out = (void *)malloc(sizeof(EstadoEstacionamento));
    return ts;
}

void start_thread(ThreadState *ts)
{
    log_print("[THREAD_EXEC] start_thread()\n", LEVEL_DEBUG);
    if (ts == NULL)
    {
        log_print("[THREAD_EXEC] [ERROR] [Thread exec] ERROR THREAD IS NULL", LEVEL_ERROR);
    }
    if (!ts->__created)
    {
        ThreadCallParams *args = (ThreadCallParams *)malloc(sizeof(ThreadCallParams));
        args->ts = ts;
        // memcpy(args->ts, ts, sizeof(ThreadState));
        args->param = ts->args;
        // memcpy(args->param, ts->args, sizeof(void*));
        log_print("[THREAD_EXEC] 3 start_thread()\n", LEVEL_DEBUG);

        printf("%p %lu %p %p\n", &ts->thread_id, ts->thread_id, ts->args, thread_procedure);
        fflush(NULL);

        pthread_t temp_tt;
        pthread_create(&temp_tt, NULL, thread_procedure, args);
        log_print("[THREAD_EXEC] 5 start_thread()\n", LEVEL_DEBUG);
        ts->thread_id = temp_tt;

        // log_print("[THREAD_EXEC] 4 start_thread()\n", LEVEL_DEBUG);
        ts->__created = 1;
    }
    log_print("[THREAD_EXEC] fim start_thread()\n", LEVEL_DEBUG);
}

void wait_thread_end(ThreadState *ts)
{
    pthread_join(ts->thread_id, NULL);
}
