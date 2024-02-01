#ifndef ASYNC_CONTEXT
#define ASYNC_CONTEXT

#include <stdint.h>

typedef struct AsyncContext AsyncContext;

AsyncContext *create_async_context();
void release_async_context(AsyncContext *);

void execute(AsyncContext *);
void schedule_task(AsyncContext *, void (*task)());

#endif