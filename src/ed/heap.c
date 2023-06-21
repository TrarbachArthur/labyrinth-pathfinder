#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

#define START_SIZE 5

typedef struct {
    double priority;
    void* data;
} Node;

struct Heap {
    int capacity;
    int size;
    Node* nodes;
};

Heap *heap_construct() {
    Heap *heap = calloc(1, sizeof(Heap));
    heap->capacity = START_SIZE;
    heap->size = 0;
    heap->nodes = calloc(heap->capacity, sizeof(Node));

    return heap;
}

void heap_print(Heap *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%lf %p -> ", heap->nodes[i].priority, heap->nodes[i].data);
    }
    printf("\n");
}

void heapifyUp(Heap *heap) {
    int pos = heap->size;
    int parentPos = (int) (pos-1) / 2;
    Node aux = heap->nodes[pos];

    while (pos > 0 && aux.priority > heap->nodes[parentPos].priority) {
        heap->nodes[pos] = heap->nodes[parentPos];
        pos = parentPos;
        parentPos = (int) (pos-1) / 2;
    }
    heap->nodes[pos] = aux;
}

void heapifyDown(Heap *heap) {
    int pos = 0, index = 0;
    Node aux = heap->nodes[pos];

    while (pos < (heap->size)/2) {
        int left = (pos*2) + 1;
        int right = (pos*2) + 2;
        index = left;
        if (right < heap->size) {
            if (heap->nodes[left].priority < heap->nodes[right].priority) {
                index = right;
            }
        }
        if (aux.priority >= heap->nodes[index].priority) break;

        printf("Troquei %d e %d\n", pos, index);

        heap->nodes[pos] = heap->nodes[index];
        pos = index; 
    }

    heap->nodes[pos] = aux;
}

void heap_push(Heap *heap, void *data, double priority) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = (Node*) realloc(heap->nodes, heap->capacity * sizeof(Node));
    }

    heap->nodes[heap->size].data = data;
    heap->nodes[heap->size].priority = priority;

    heapifyUp(heap);
    heap->size++;

    heap_print(heap);
}

bool heap_empty(Heap *heap) {
    return (!heap->size);
}

void *heap_max(Heap *heap) {
    return heap->nodes[0].data;
}

double heap_max_priority(Heap *heap) {
    return heap->nodes[0].priority;
}

void *heap_pop(Heap *heap) {
    if (heap->size <= 0) {
        printf("Heap is empty!\n");
        return NULL;
    }

    void *data = heap->nodes[0].data;
    // Setting last element as first for heapify
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    heapifyDown(heap);

    heap_print(heap);

    return data;
}

void heap_destroy(Heap *heap) {
    for (int i = 0; (i < heap->size) && heap->nodes[i].data; i++) {
        free(heap->nodes[i].data);
    }
    free(heap->nodes);
    free(heap);
}