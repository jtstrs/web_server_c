#include "async_context.h"
#include "common.h"

void execute_task(AsyncTask *task) {
    task->task();
}

AsyncContext *create_async_context() {
    AsyncContext *ctx = (AsyncContext *) malloc(sizeof(AsyncContext));

    if (!ctx) {
        return NULL;
    }

    // TODO: Probably need non trivial deleter
    List *tasks_queue = create_list(NULL);

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

void release_async_context(AsyncContext *ctx) {
    if (!ctx) {
        return;
    }

    if (ctx->tasks_queue) {
        release_list(ctx->tasks_queue);
    }

    pthread_mutex_destroy(&ctx->mut);
    free(ctx);
}

void schedule_task(AsyncContext *context, void (*task)()) {
    pthread_mutex_lock(&context->mut);
    push_list_item(context->tasks_queue, task);
    pthread_mutex_unlock(&context->mut);
}

void *event_loop(void *context) {
    AsyncContext *ctx = (AsyncContext *) context;

    while (ALWAYS) {
        pthread_mutex_lock(&ctx->mut);
        AsyncTask *task = get_last_list_item(ctx->tasks_queue);
        if (task) {
            execute_task(task);
            pop_list_item(ctx->tasks_queue);
        }
        pthread_mutex_unlock(&ctx->mut);
    }
}

void execute(AsyncContext *context) {
    if (pthread_create(&context->thread_id, NULL, event_loop, context) != 0) {
        handle_error("Cannot create thread for async context\n");
    }
    pthread_join(context->thread_id, NULL);
}
