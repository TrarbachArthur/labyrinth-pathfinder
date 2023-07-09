
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "types.h"

typedef struct Queue Queue;
typedef void (*DequeFree)(void *);

Queue *queue_construct(DequeFree free_fn);
void queue_push(Queue *queue, void *data);
bool queue_empty(Queue *queue);
void *queue_pop(Queue *queue);
void queue_destroy(Queue *queue);

#endif