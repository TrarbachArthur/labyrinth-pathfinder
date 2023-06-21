#include <stdlib.h>
#include "deque.h"
#include "queue.h"

struct Queue {
    Deque* deque;
};

Queue *queue_construct() {
    Queue *queue = calloc(1, sizeof(Queue));
    queue->deque = deque_construct();

    return queue;
}

void queue_push(Queue *queue, void *data) {
    deque_push_back(queue->deque, data);
}

bool queue_empty(Queue *queue) {
    return (deque_size(queue->deque) == 0);
}

void *queue_pop(Queue *queue) {
    return deque_pop_front(queue->deque);
}

void queue_destroy(Queue *queue) {
    deque_destroy(queue->deque);
    free(queue);
}