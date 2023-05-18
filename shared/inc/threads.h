#ifndef SHARED_THREADS_H
#define SHARED_THREADS_H 1

#include <stdlib.h>
#include <pthread.h>

#define STANDARD_THREAD_SIZE 10000

struct ThreadSharedMemory
{
    void *in; // shared memory, read-only to thread
    int in_locked;

    void *out; // shared memory, read-only to thread caller
    int out_locked;
};
typedef struct ThreadSharedMemory ThreadSharedMemory;

struct ThreadState
{
    pthread_t thread_id;
    int __created; // read-only for thread
    int __running; // read-only for thread caller

    void (*routine)(struct ThreadState *ts, void *args); // You have to type all void*
    void *(*optional_before)(struct ThreadState *ts, void *args);
    void *(*optional_after)(struct ThreadState *ts, void *args);

    ThreadSharedMemory *memory;
    void *args; // arguments sent to function as void*
    // but please notice that function receives a ThreadCallParams*

    // keep in mind the thread is a 2 way relationship
    // if you need more, same rules apply for each relationship
};
typedef struct ThreadState ThreadState;

struct ThreadCallParams
{
    ThreadState *ts;
    void *param;
};
typedef struct ThreadCallParams ThreadCallParams;

// may set message size to -1 for standard frame size
ThreadState *create_thread_state(size_t message_size);

void start_thread(ThreadState *ts);

void wait_thread_end(ThreadState *ts);

#endif