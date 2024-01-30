#include "async_context.h"
#include "../containers/list.h"
#include "common.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct AsyncContext {
    struct List *tasks_queue;
    pthread_t thread_id;
    pthread_mutex_t mut;
};

struct AsyncTask {
    int32_t task_id;
    void (*task)();
};

void execute_task(struct AsyncTask *task) {
    task->task();
}

struct AsyncContext *create_async_context() {
    struct AsyncContext *ctx = (struct AsyncContext *) malloc(sizeof(struct AsyncContext));

    if (!ctx) {
        return NULL;
    }

    struct List *tasks_queue = create_list();

    if (!tasks_queue) {
        free(ctx);
        return NULL;
    }

    if (pthread_mutex_init(&ctx->mut, NULL) != 0) {
        release_list(tasks_queue);
        free(ctx);
        return NULL;
    }

    ctx->tasks_queue = tasks_queue;
    ctx->thread_id = 0;
}

void release_async_context(struct AsyncContext *ctx) {
    if (!ctx) {
        return;
    }

    if (ctx->tasks_queue) {
        release_list(ctx->tasks_queue);
    }

    pthread_mutex_destroy(&ctx->mut);
    free(ctx);
}

void schedule_task(struct AsyncContext *context, void (*task)()) {
    pthread_mutex_lock(&context->mut);
    push_item(context->tasks_queue, task);
    pthread_mutex_unlock(&context->mut);
}

void *event_loop(void *context) {
    struct AsyncContext *ctx = (struct AsyncContext *) context;

    while (ALWAYS) {
        pthread_mutex_lock(&ctx->mut);
        struct AsyncTask *task = last_item(ctx->tasks_queue);
        if (task) {
            execute_task(task);
            pop_item(ctx->tasks_queue);
        }
        pthread_mutex_unlock(&ctx->mut);
    }
}

void execute(struct AsyncContext *context) {
    if (pthread_create(&context->thread_id, NULL, event_loop, context) != 0) {
        handle_error("Cannot create thread for async context\n");
    }
    pthread_join(context->thread_id, NULL);
}
