
#include <stdio.h>
#include <stdlib.h>
#include "forward_list.h"
#include "hash.h"

struct HashTable {
    int table_size, used_buckets, num_elem;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    ForwardList **buckets;
};

typedef struct HashTableIterator HashTableIterator;

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn) {
    HashTable *h = calloc(1, sizeof(HashTable));
    h->table_size = table_size;
    h->used_buckets = h->num_elem = 0;
    h->hash_fn = hash_fn;
    h->cmp_fn = cmp_fn;
    h->buckets = calloc(h->table_size, sizeof(ForwardList*));

    return h;
}

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void *hash_table_set(HashTable *h, void *key, void *val) {
    int idx = h->hash_fn(h, key);
    HashTableItem *item;
    if (h->buckets[idx]) {
        item = (HashTableItem*) forward_list_find(l, key, h->cmp_fn);
        if (item) {
            void *aux = item->val;
            item->val = val;
            return aux;
        }
    }
    else {
        h->buckets[idx] = forward_list_construct();
        h->used_buckets++;
    }

    item = calloc(1, sizeof(HashTableItem));
    item->key = key;
    item->val = val;
    forward_list_push_front(h->buckets[idx], item);
    h->num_elem++;

    return NULL;
}

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key) {
    int idx = h->hash_fn(h, key);
    ForwardList *bucket = h->buckets[idx];

    HashTableItem *item = forward_list_find(bucket, key, h->cmp_fn);

    if (item) return item->val;

    return NULL;
}

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key) {
    HashTableItem *item = hash_table_get(h, key);
    int idx = h->hash_fn(h, key);
    void *val = NULL;

    if (item) val = item->val;

    forward_list_remove(h->buckets[idx], item);
    h->num_elem--;

    if (forward_list_size(h->buckets[idx]) == 0) {
        h->used_buckets--;
        free(h->buckets[idx];
    }
    
    return val;
}

// numero de buckets
int hash_table_size(HashTable *h) {
    return h->used_buckets;
}

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h) {
    return h->num_elem;
}

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h) {
    for (int i = 0; i < h->table_size; i++) {
        if (h->buckets[i]) {
            forward_list_destroy(h->buckets[i]);
        }
    }
    free(h->buckets);
    free(h);
}

// cria um novo iterador para a tabela hash
HashTableIterator *hash_table_iterator(HashTable *h);

// retorna 1 se o iterador chegou ao fim da tabela hash ou 0 caso contrario
int hash_table_iterator_is_over(HashTableIterator *it);

// retorna o proximo par chave valor da tabela hash
HashTableItem *hash_table_iterator_next(HashTableIterator *it);

// desaloca o iterador da tabela hash
void hash_table_iterator_destroy(HashTableIterator *it);