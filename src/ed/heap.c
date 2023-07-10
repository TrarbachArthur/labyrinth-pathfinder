#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

#define START_SIZE 5

typedef struct {
    double priority;
    void *data;
} Node;

struct Heap {
    int capacity;
    int size;
    Node *nodes;
    HashTable *ht;
};

Heap *heap_construct(HashTable *h) {
    Heap *heap = calloc(1, sizeof(Heap));
    heap->capacity = START_SIZE;
    heap->size = 0;
    heap->nodes = calloc(heap->capacity, sizeof(Node));
    heap->ht = h;

    return heap;
}

void heap_print(Heap *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%lf %p -> ", heap->nodes[i].priority, heap->nodes[i].data);
    }
    printf("\n");
}

void heapifyUp(Heap *heap, int idx) {
    int pos = idx;
    int parentPos = (int) (pos-1) / 2;
    Node aux = heap->nodes[pos];

    while (pos > 0 && aux.priority < heap->nodes[parentPos].priority) {
        heap->nodes[pos] = heap->nodes[parentPos];
        
        int *new_idx = calloc(1, sizeof(int));
        *new_idx = pos;
        void *tmp_idx = hash_table_set(heap->ht, heap->nodes[pos].data, new_idx);
        if (tmp_idx) {
            free(tmp_idx);
        }

        pos = parentPos;
        parentPos = (int) (pos-1) / 2;
    }
    heap->nodes[pos] = aux;

    int *new_idx = calloc(1, sizeof(int));
    *new_idx = pos;
    void *tmp_idx = hash_table_set(heap->ht, heap->nodes[pos].data, new_idx);
    if (tmp_idx) {
        free(tmp_idx);
    }
}

void heapifyDown(Heap *heap) {
    int pos = 0, index = 0;
    Node aux = heap->nodes[pos];

    while (pos < (heap->size)/2) {
        int left = (pos*2) + 1;
        int right = (pos*2) + 2;
        index = left;
        if (right < heap->size) {
            if (heap->nodes[left].priority > heap->nodes[right].priority) {
                index = right;
            }
        }
        if (aux.priority <= heap->nodes[index].priority) break;

        //printf("Troquei %d e %d\n", pos, index);

        heap->nodes[pos] = heap->nodes[index];

        int *new_idx = calloc(1, sizeof(int));
        *new_idx = pos;
        void *tmp_idx = hash_table_set(heap->ht, heap->nodes[pos].data, new_idx);
        if (tmp_idx) {
            free(tmp_idx);
        }

        pos = index; 
    }

    heap->nodes[pos] = aux;

    int *new_idx = calloc(1, sizeof(int));
    *new_idx = pos;
    void *tmp_idx = hash_table_set(heap->ht, heap->nodes[pos].data, new_idx);
    if (tmp_idx) {
        free(tmp_idx);
    }
}

void _hash_table_set(HashTable *h, void *data, int index){
    int *new_index = (int *)malloc(sizeof(int));
    *new_index = index;
    new_index = hash_table_set(h, data, new_index);
    if (new_index != NULL)
        free(new_index); 
}

void* heap_push(Heap *heap, void *data, double priority) {
    int *index = (int *)hash_table_get(heap->ht, data);

    if (index) {
        //printf("Got %lf %lf\n", heap->nodes[*index].priority, priority);
        if (priority < heap->nodes[*index].priority) {
            void* aux = heap->nodes[*index].data;
            heap->nodes[*index].priority = priority;
            heap->nodes[*index].data = data;

            int *old_val = hash_table_pop(heap->ht, aux);

            void *tmp_idx = hash_table_set(heap->ht, data, old_val);
            if (tmp_idx) {
                free(tmp_idx);
            }

            heapifyUp(heap, *index);
            data = aux;
        }
        //printf("Repeated\n");
        return data;
    }
    
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = (Node*) realloc(heap->nodes, heap->capacity * sizeof(Node));
    }

    heap->nodes[heap->size].data = data;
    heap->nodes[heap->size].priority = priority;

    int *new_idx = calloc(1, sizeof(int));
    *new_idx = heap->size;
    void *tmp_idx = hash_table_set(heap->ht, heap->nodes[*new_idx].data, new_idx);
    if (tmp_idx) {
        free(tmp_idx);
    }

    heapifyUp(heap, heap->size);
    heap->size++;

    //heap_print(heap);

    return NULL;
}

bool heap_empty(Heap *heap) {
    return (!heap->size);
}

void *heap_max(Heap *heap) {
    return heap->nodes[0].data;
}

double heap_min_priority(Heap *heap) {
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
    
    void *aux = hash_table_pop(heap->ht, data);
    
    if (aux) {
        free(aux);
    }

    heapifyDown(heap);

    //heap_print(heap);

    return data;
}

void heap_destroy(Heap *heap) {
    for (int i = 0; (i < heap->size) && heap->nodes[i].data; i++) {
        free(heap->nodes[i].data);
    }
    free(heap->nodes);
    free(heap);
}