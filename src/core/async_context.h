#ifndef ASYNC_CONTEXT
#define ASYNC_CONTEXT

#include <stdint.h>

struct AsyncContext;

struct AsyncContext *create_async_context();
void release_async_context(struct AsyncContext *);

void execute(struct AsyncContext *);
void schedule_task(struct AsyncContext *, void (*task)());

#endif