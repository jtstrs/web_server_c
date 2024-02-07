#ifndef ASYNC_CONTEXT
#define ASYNC_CONTEXT

#include "../containers/list.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct AsyncContext {
    List *tasks_queue;
    pthread_t thread_id;
    pthread_mutex_t mut;
} AsyncContext;

typedef struct AsyncTask {
    int32_t task_id;
    void (*task)();
} AsyncTask;

AsyncContext *create_async_context();
void release_async_context(AsyncContext *);

void execute(AsyncContext *);
void schedule_task(AsyncContext *, void (*task)());

#endif