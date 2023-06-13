#include <stdlib.h>
#include "deque.h"

typedef struct node {
    void* data;
    struct node *next;
    struct node *prev;
} Node;

Node *node_construct(void* data) {
    Node* node = calloc(1, sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void node_destroy(Node* node) {
    free(node);
}

struct Deque {
    Node *first;
    Node *last;
    size_t size;
}

// criacao do deque
Deque *deque_construct(void) {
    Deque *deque = calloc(1, sizeof(Deque));
    deque->first = NULL;
    deque->last = NULL;
    deque->size = 0;

    return deque;
}

// funcoes para insercao na direita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, void *val) {
    Node *new_node = node_construct(val);

    if (d->first == NULL) {
        d->first = d->last = new_node;
    }
    else {
        new_node->prev = d->last;
        d->last = d->last->next = new_node;
    }
    d->size++;
}
void deque_push_front(Deque *d, void *val) {
    Node *new_node = node_construct(val);

    if (d->first = NULL) {
        d->first = d->last = new_node;
    }
    else {
        new_node->next = d->first;
        d->first = d->first->prev = new_node;
    }
    d->size++;
}

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
void *deque_pop_back(Deque *d) {
    Node *aux;
    void *data;

    if (d->size == 0) {
        printf("Error: Deque is empty!\n")
        return NULL;
    }
    
    aux = d->last
    data = aux->data;
    
    if (d->first == d->last) {
        d->first = d->last = NULL;
    }
    else {
        d->last = aux->prev;
    }
    d->size--;

    free(aux);
    

    return data;
}
void *deque_pop_front(Deque *d) {
    Node *aux;
    void *data;

    if (d->size == 0) {
        printf("Error: Deque is empty!\n")
        return NULL;
    }

    aux = d->first
    data = aux->data;
    
    if (d->first == d->last) {
        d->first = d->last = NULL;
    }
    else {
        d->first = aux->next;
    }
    d->size--;

    free(aux);

    return data;
}

// numero de elementos
int deque_size(Deque *d) {
    return d->size;
}

// obtem o i-esimo elemento
void *deque_get(Deque *d, int idx) {
    if (d->size <= idx) {
        printf("Error: Index out of bounds.\n");
        return NULL;
    }

    if (idx < (d->size/2)) {
        Node *node = d->first;
        for (int i = 0; i < idx; i++) {
            node = node->next;
        }
    }
    else {
        Node *node = d->last;
        for (int i = d->size-1; i > idx; i--) {
            node = node->prev;
        }
    }

    return node->data;
}

// libera o espaco alocado para o deque
void deque_destroy(Deque *d) {
    while (d->size > 0) {
        free(deque_pop_back(d));
    }
    free(d);
}