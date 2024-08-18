/*
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Author: Iuri Sônego Cardoso
 * Date: 24 Jun 2024
 */
#include "lineards.h"
#include <stdlib.h>
#include <string.h>

#define PRINTREP(file, ch, times) {int i; for(i=0; i<times; i++) putc(ch, file); }

/* Estrutura para lista encadeada */
typedef struct Node {
    void *data;
    struct Node *next;
} Node;

/* Estrutura do iterador. */
typedef struct LDSIterator {
    LINEAR_DS * ds;
    size_t position;
    Node * current;
    Node * previous;
    lds_return_t (*add)(LDS_ITERATOR *it, void *value);
    lds_return_t (*next)(LDS_ITERATOR *it);
    lds_return_t (*get)(LDS_ITERATOR *it, void *element);
    lds_return_t (*set)(LDS_ITERATOR *it, void *value);
    lds_return_t (*remove)(LDS_ITERATOR *it, void*removed_element);
    lds_return_t (*reset)(LDS_ITERATOR *it);
    lds_return_t (*go)(LDS_ITERATOR *it, size_t position);
} LDSIterator;

/* Definição da estrutura de dados oculta */
typedef struct LinearDS {
    union {
        struct {
            void *vector;
            size_t head; /* Índice de início da fila circular */
            size_t tail; /* Índice de fim da fila circular */
        };
        struct {
            Node *first;
            Node *last;
        } list;
    } storage;
    size_t size;
    size_t capacity;
    size_t data_size;
    lds_type_t type;
    LDSIterator iterator;

#ifndef NDEBUG
    /* Debug */
    FILE * debug_log;
    void (*debug_fmt)(FILE * out, void *value);
#endif /* NDEBUG */

    /* Ponteiros para funções */
    lds_return_t (*insert)(LINEAR_DS *ds, size_t position, void *value);
    lds_return_t (*remove)(LINEAR_DS *ds, size_t position, void *removed_element);
    lds_return_t (*get)(LINEAR_DS *ds, size_t position, void *element);
    lds_return_t (*set)(LINEAR_DS *ds, size_t position, void *value);
    void (*free)(LINEAR_DS *ds); /* Ponteiro para a função de liberação de memória */
} LinearDS;

/* Funções para manipulação da estrutura de dados */
static lds_return_t insert_element_in_vector(LINEAR_DS *ds, size_t position, void *value);
static lds_return_t insert_element_in_list(LINEAR_DS *ds, size_t position, void *value);
static lds_return_t remove_element_from_vector(LINEAR_DS *ds, size_t position, void *removed_element);
static lds_return_t remove_element_from_list(LINEAR_DS *ds, size_t position, void *removed_element);
static lds_return_t get_element_from_vector(LINEAR_DS *ds, size_t position, void *element);
static lds_return_t get_element_from_list(LINEAR_DS *ds, size_t position, void *element);
static lds_return_t set_element_in_vector(LINEAR_DS *ds, size_t position, void *value);
static lds_return_t set_element_in_list(LINEAR_DS *ds, size_t position, void *value);
static void free_vector(LINEAR_DS *ds);
static void free_list(LINEAR_DS *ds);

/* Funções para manipulação do iterador */
static lds_return_t it_add_in_vector(LDS_ITERATOR *it, void *value);
static lds_return_t it_add_in_list(LDS_ITERATOR *it, void *value);
static lds_return_t it_next_in_vector(LDS_ITERATOR *it);
static lds_return_t it_next_in_list(LDS_ITERATOR *it);
static lds_return_t it_get_from_vector(LDS_ITERATOR *it, void *element);
static lds_return_t it_get_from_list(LDS_ITERATOR *it, void *element);
static lds_return_t it_remove_from_vector(LDS_ITERATOR *it, void*removed_element);
static lds_return_t it_remove_from_list(LDS_ITERATOR *it, void*removed_element);
static lds_return_t it_reset_in_vector(LDS_ITERATOR *it);
static lds_return_t it_reset_in_list(LDS_ITERATOR *it);
static lds_return_t it_go_in_vector(LDS_ITERATOR *it, size_t position);
static lds_return_t it_go_in_list(LDS_ITERATOR *it, size_t position);
static lds_return_t it_set_in_vector(LDS_ITERATOR *it, void *element);
static lds_return_t it_set_in_list(LDS_ITERATOR *it, void *element);

#ifdef NDEBUG
#define print_debug(ds, action);
#else
/* Funções de debug */
void lds_debug(LINEAR_DS * ds, FILE * out, void (*debug_fmt)(FILE * out, void *value)) {
    ds->debug_log = out;
    ds->debug_fmt = debug_fmt;
}
static void print_debug(LINEAR_DS * ds, char action[]) {
    if (ds->debug_log != NULL && ds->debug_fmt != NULL) {
        putc('\n', ds->debug_log);
        int c = fprintf(ds->debug_log, "(%p):%s ", ds, action);
        PRINTREP(ds->debug_log, '=', 80-c);
        putc('\n', ds->debug_log);

        if (ds->type == LDS_VECTOR) {
            fprintf(ds->debug_log, "type: LDS_VECTOR; size: %llu; capacity: %llu; head: %llu; tail: %llu\n",
                   ds->size, ds->capacity, ds->storage.head, ds->storage.tail);
        }
        else {
            fprintf(ds->debug_log, "type: LDS_LINKED_LIST; size: %llu\n",
                   ds->size);
        }
        PRINTREP(ds->debug_log, '-', 80);
        putc('\n', ds->debug_log);

        if (ds->type == LDS_VECTOR) {
            size_t i;
            for(i=0; i<ds->size; i++) {
                void * v = (char *)ds->storage.vector + ((ds->storage.head + i) % ds->capacity) * ds->data_size;
                ds->debug_fmt(ds->debug_log,v);
            }
        }
        else {
            size_t i;
            Node *current = ds->storage.list.first;
            for(i=0; i<ds->size; i++) {
                ds->debug_fmt(ds->debug_log,current->data);
                current = current->next;
            }
        }
        putc('\n', ds->debug_log);
        PRINTREP(ds->debug_log, '=', 80);
        putc('\n', ds->debug_log);
    }
}
#endif /* NDEBUG */


/* Implementação das funções de manipulação */
LINEAR_DS* lds_new_vector(size_t initial_capacity, size_t data_size) {
    LINEAR_DS *ds = (LINEAR_DS*)malloc(sizeof(LINEAR_DS));
    if (ds == NULL) {
        return NULL; /* Falha ao alocar memória */
    }
    ds->storage.vector = malloc(initial_capacity * data_size);
    if (ds->storage.vector == NULL) {
        free(ds);
        return NULL; /* Falha ao alocar memória */
    }
    ds->size = 0;
    ds->capacity = initial_capacity;
    ds->data_size = data_size;
    ds->type = LDS_VECTOR;
    ds->storage.head = 0;
    ds->storage.tail = 0;
#ifndef NDEBUG
    ds->debug_log = NULL;
#endif

    /* Inicializa os ponteiros para as funções */
    ds->insert = insert_element_in_vector;
    ds->remove = remove_element_from_vector;
    ds->get = get_element_from_vector;
    ds->set = set_element_in_vector;
    ds->free = free_vector;

    /* Inicializa o iterador */
    ds->iterator.ds = ds;
    ds->iterator.position = 0;
    ds->iterator.current = NULL;
    ds->iterator.previous = NULL;
    ds->iterator.add = it_add_in_vector;
    ds->iterator.next = it_next_in_vector;
    ds->iterator.get = it_get_from_vector;
    ds->iterator.set = it_set_in_vector;
    ds->iterator.remove = it_remove_from_vector;
    ds->iterator.reset = it_reset_in_vector;
    ds->iterator.go = it_go_in_vector;

    print_debug(ds, "lds_new_vector");
    return ds;
}

LINEAR_DS* lds_new_list(size_t data_size) {
    LINEAR_DS *ds = (LINEAR_DS*)malloc(sizeof(LINEAR_DS));
    if (ds == NULL) {
        return NULL; /* Falha ao alocar memória */
    }
    ds->storage.list.first = NULL;
    ds->storage.list.last = NULL;
    ds->size = 0;
    ds->data_size = data_size;
    ds->type = LDS_LINKED_LIST;
#ifndef NDEBUG
    ds->debug_log = NULL;
#endif
    /* Inicializa os ponteiros para as funções */
    ds->insert = insert_element_in_list;
    ds->remove = remove_element_from_list;
    ds->get = get_element_from_list;
    ds->set = set_element_in_list;
    ds->free = free_list;

    /* Inicializa o iterador */
    ds->iterator.ds = ds;
    ds->iterator.position = 0;
    ds->iterator.current = NULL;
    ds->iterator.previous = NULL;
    ds->iterator.add = it_add_in_list;
    ds->iterator.next = it_next_in_list;
    ds->iterator.get = it_get_from_list;
    ds->iterator.set = it_set_in_list;
    ds->iterator.remove = it_remove_from_list;
    ds->iterator.reset = it_reset_in_list;
    ds->iterator.go = it_go_in_list;

    print_debug(ds, "lds_new_list");
    return ds;
}

lds_return_t lds_insert(LINEAR_DS *ds, size_t position, void *value) {
    if (ds == NULL || value == NULL) {
        return LDS_NULL;
    }
    if (position > ds->size || position < 0) {
        return LDS_POS_ERR;
    }
    lds_return_t r = ds->insert(ds, position, value);
    print_debug(ds, "lds_insert");
    return r;
}


lds_return_t lds_insert_last(LINEAR_DS *ds, void *value) {
    return lds_insert(ds, ds->size, value);
}

lds_return_t lds_get(LINEAR_DS *ds, size_t position, void *element) {
    if (ds == NULL || element == NULL) {
        return LDS_NULL;
    }
    if (position >= ds->size || position < 0) {
        return LDS_POS_ERR;
    }
    lds_return_t r = ds->get(ds, position, element);
    print_debug(ds, "lds_get");
    return r;
}

lds_return_t lds_set(LINEAR_DS *ds, size_t position, void *value) {
    if (ds == NULL || value == NULL) {
        return LDS_NULL;
    }
    if (position >= ds->size || position < 0) {
        return LDS_POS_ERR;
    }
    lds_return_t r = ds->set(ds, position, value);
    print_debug(ds, "lds_set");
    return r;
}

lds_return_t lds_remove(LINEAR_DS *ds, size_t position, void *removed_element) {
    if (ds == NULL) {
        return LDS_NULL;
    }
    if (position >= ds->size || position < 0) {
        return LDS_POS_ERR;
    }
    lds_return_t r = ds->remove(ds, position, removed_element);
    print_debug(ds, "lds_remove");
    return r;
}

lds_return_t lds_remove_last(LINEAR_DS *ds, void *removed_element) {
    if (ds == NULL) {
        return LDS_NULL;
    }
    lds_return_t r = ds->remove(ds, ds->size-1, removed_element);
    print_debug(ds, "lds_remove_last");
    return r;
}

void lds_free(LINEAR_DS *ds) {
    print_debug(ds, "lds_free");
    if (ds != NULL) {
        ds->free(ds);
        free(ds);
    }
}

/* Funções específicas para vetor */
static lds_return_t insert_element_in_vector(LINEAR_DS *ds, size_t position, void *value) {
    if (ds->size == ds->capacity) {
        size_t new_capacity = ds->capacity > 0 ? ds->capacity * 2 : 1;
        void *new_vector = realloc(ds->storage.vector, new_capacity * ds->data_size);
        if (new_vector == NULL) {
            return LDS_FAIL; /* Falha ao realocar memória */
        }

        /* Reorganiza o vetor caso o fim esteja antes do início. */
        if (ds->storage.head >= ds->storage.tail) {
            memmove((char*)new_vector + ds->capacity * ds->data_size,
                    new_vector, ds->storage.tail * ds->data_size);
            ds->storage.tail = ds->capacity + ds->storage.tail;
        }

        ds->storage.vector = new_vector;
        ds->capacity = new_capacity;
    }

    if (position == ds->size) {
        memcpy((char*)ds->storage.vector + (ds->storage.tail * ds->data_size), value, ds->data_size);
        ds->storage.tail = (ds->storage.tail + 1) % ds->capacity;
    } else if (position == 0) {
        ds->storage.head = (ds->storage.head - 1 + ds->capacity) % ds->capacity;
        memcpy((char*)ds->storage.vector + ds->storage.head * ds->data_size, value, ds->data_size);
    } else {
        size_t index = (ds->storage.head + position) % ds->capacity;
        size_t tail = ds->storage.tail;

        /* Fim está depois da inserção. */
        if (tail > index) {
            memmove((char*)ds->storage.vector + (index + 1) * ds->data_size,
                    (char*)ds->storage.vector + index * ds->data_size,
                    (tail-index) * ds->data_size);
        }

        /* Fim está antes da inserção */
        else {
            /* Movimenta o começo para a direita */
            memmove((char*)ds->storage.vector + 1 * ds->data_size,
                    (char*)ds->storage.vector,
                    tail * ds->data_size);
            /* Movimenta o último para o primeiro. */
            memmove((char*)ds->storage.vector,
                    (char*)ds->storage.vector + (ds->capacity - 1) * ds->data_size,
                    ds->data_size);
            /* Movimenta o fim para a direita. */
            memmove((char*)ds->storage.vector + (index + 1) * ds->data_size,
                    (char*)ds->storage.vector + index * ds->data_size,
                    (ds->capacity - index - 1) * ds->data_size);
        }

        memcpy((char*)ds->storage.vector + index * ds->data_size,
               value, ds->data_size);
        ds->storage.tail = (ds->storage.tail + 1) % ds->capacity;
    }
    ds->size++;
    return LDS_SUCCESS;
}

static lds_return_t remove_element_from_vector(LINEAR_DS *ds, size_t position, void *removed_element) {
    if (removed_element != NULL) {
        memcpy(removed_element, (char*)ds->storage.vector + ((ds->storage.head + position) % ds->capacity) * ds->data_size, ds->data_size);
    }

    if (position == ds->size - 1) {
        ds->storage.tail = (ds->storage.tail - 1 + ds->capacity) % ds->capacity;
    } else if (position == 0) {
        ds->storage.head = (ds->storage.head + 1) % ds->capacity;
    } else {
        size_t index = (ds->storage.head + position) % ds->capacity;
        size_t tail = ds->storage.tail;

        /* Move à esquerda */
        if (tail > index) {
            memmove((char*)ds->storage.vector + index * ds->data_size,
                    (char*)ds->storage.vector + (index + 1) * ds->data_size,
                    (tail - index - 1) * ds->data_size);
            ds->storage.tail = (ds->storage.tail - 1 + ds->capacity) % ds->capacity;
        } else {
            memmove((char*)ds->storage.vector + (ds->storage.head + 1) * ds->data_size,
                    (char*)ds->storage.vector + ds->storage.head * ds->data_size,
                    position * ds->data_size);
            ds->storage.head = (ds->storage.head + 1) % ds->capacity;
        }
    }
    ds->size--;
    return LDS_SUCCESS;
}

static lds_return_t get_element_from_vector(LINEAR_DS *ds, size_t position, void *element) {
    memcpy(element, (char*)ds->storage.vector + ((ds->storage.head + position) % ds->capacity) * ds->data_size, ds->data_size);
    return LDS_SUCCESS;
}

static lds_return_t set_element_in_vector(LINEAR_DS *ds, size_t position, void *value) {
    if (memcmp(value, (char*)ds->storage.vector + ((ds->storage.head + position) % ds->capacity) * ds->data_size, ds->data_size) == 0) {
        return LDS_FAIL;
    }

    memcpy((char*)ds->storage.vector + ((ds->storage.head + position) % ds->capacity) * ds->data_size, value, ds->data_size);
    return LDS_SUCCESS;
}

static void free_vector(LINEAR_DS *ds) {
    free(ds->storage.vector);
}

static lds_return_t insert_element_in_list(LINEAR_DS *ds, size_t position, void *value) {
    lds_return_t r = it_go_in_list(&ds->iterator, position);
    if (r == LDS_SUCCESS) {
        return it_add_in_list(&ds->iterator, value);
    }
    return r;
}

static lds_return_t remove_element_from_list(LINEAR_DS *ds, size_t position, void *removed_element) {
    lds_return_t r = it_go_in_list(&ds->iterator, position);
    if (r == LDS_SUCCESS) {
        return it_remove_from_list(&ds->iterator, removed_element);
    }
    return r;
}

static lds_return_t get_element_from_list(LINEAR_DS *ds, size_t position, void *element) {
    lds_return_t r = it_go_in_list(&ds->iterator, position);
    if (r == LDS_SUCCESS) {
        return it_get_from_list(&ds->iterator, element);
    }
    return r;
}

static lds_return_t set_element_in_list(LINEAR_DS *ds, size_t position, void *value) {
    lds_return_t r = it_go_in_list(&ds->iterator, position);
    if (r == LDS_SUCCESS) {
        return it_set_in_list(&ds->iterator, value);
    }
    return r;
}

static void free_list(LINEAR_DS *ds) {
    Node *current = ds->storage.list.first;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

/* Funções para consultar os campos da estrutura */
size_t lds_size(LINEAR_DS *ds) {
    return ds != NULL ? ((LINEAR_DS*)ds)->size : 0;
}

int lds_empty(LINEAR_DS *ds) {
    return ds != NULL ? ds->size == 0 : 0;
}

size_t lds_capacity(LINEAR_DS *ds) {
    return (ds != NULL || ds->type == LDS_VECTOR) ? ((LINEAR_DS*)ds)->capacity : 0;
}

size_t lds_data_size(LINEAR_DS *ds) {
    return ds != NULL ? ((LINEAR_DS*)ds)->data_size : 0;
}

lds_type_t lds_type(LINEAR_DS *ds) {
    return ds != NULL ? ((LINEAR_DS*)ds)->type : LDS_UNKNOWN;
}

/* Funções de iterador */
LDS_ITERATOR * lds_iterator(LINEAR_DS *ds) {
    return &ds->iterator;
}

size_t lds_it_position(LDS_ITERATOR *it) {
    return it->position;
}

lds_return_t lds_it_add(LDS_ITERATOR *it, void *value) {
    if (it == NULL || value == NULL) {
        return LDS_NULL;
    }
    return it->add(it, value);
}

lds_return_t lds_it_next(LDS_ITERATOR *it) {
    if (it == NULL) {
        return LDS_NULL;
    }
    if (it->position == it->ds->size) {
        return LDS_POS_ERR;
    }
    return it->next(it);
}

lds_return_t lds_it_has_next(LDS_ITERATOR *it) {
    if (it == NULL) {
        return LDS_NULL;
    }
    return (it->position < it->ds->size) ? LDS_SUCCESS : LDS_FAIL;
}

lds_return_t lds_it_get(LDS_ITERATOR *it, void *element) {
    if (it == NULL || element == NULL) {
        return LDS_NULL;
    }
    if (it->position == it->ds->size) {
        return LDS_POS_ERR;
    }
    return it->get(it, element);
}


lds_return_t lds_it_set(LDS_ITERATOR *it, void *value) {
    if (it == NULL || value == NULL) {
        return LDS_NULL;
    }
    if (it->position == it->ds->size) {
        return LDS_POS_ERR;
    }
    return it->set(it, value);
}

lds_return_t lds_it_remove(LDS_ITERATOR *it, void *removed_element) {
    if (it == NULL) {
        return LDS_NULL;
    }
    if (it->position == it->ds->size) {
        return LDS_POS_ERR;
    }
    return it->remove(it, removed_element);
}

lds_return_t lds_it_reset(LDS_ITERATOR *it) {
    if (it == NULL) {
        return LDS_NULL;
    }
    return it->reset(it);
}


lds_return_t lds_it_go(LDS_ITERATOR *it, size_t pos) {
    if (it == NULL) {
        return LDS_NULL;
    }
    if (pos > it->ds->size) {
        return LDS_POS_ERR;
    }
    return it->go(it, pos);
}

static lds_return_t it_add_in_vector(LDS_ITERATOR *it, void *value) {
    return insert_element_in_vector(it->ds, it->position, value);
}

static lds_return_t it_add_in_list(LDS_ITERATOR *it, void *value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return LDS_FAIL;
    }
    new_node->data = malloc(it->ds->data_size);
    if (new_node->data == NULL) {
        free(new_node);
        return LDS_FAIL;
    }
    memcpy(new_node->data, value, it->ds->data_size);

    new_node->next = it->current;
    if (it->previous != NULL) {
        it->previous->next = new_node;
    }
    it->current = new_node;

    if (it->position == 0) {
        it->ds->storage.list.first = new_node;
    }

    if (it->position == it->ds->size) {
        it->ds->storage.list.last = new_node;
    }

    it->ds->size++;
    return LDS_SUCCESS;
}

static lds_return_t it_next_in_vector(LDS_ITERATOR *it) {
    it->position++;
    return LDS_SUCCESS;
}

static lds_return_t it_next_in_list(LDS_ITERATOR *it) {
    it->position++;
    it->previous = it->current;
    it->current = it->current->next;
    return LDS_SUCCESS;
}

static lds_return_t it_get_from_vector(LDS_ITERATOR *it, void *element) {
    return get_element_from_vector(it->ds, it->position, element);
}

static lds_return_t it_get_from_list(LDS_ITERATOR *it, void *element) {
    if (it->position >= it->ds->size) {
        return LDS_POS_ERR;
    }
    memcpy(element, it->current->data, it->ds->data_size);
    return LDS_SUCCESS;
}

static lds_return_t it_remove_from_vector(LDS_ITERATOR *it, void *removed_element) {
    return remove_element_from_vector(it->ds, it->position, removed_element);
}

static lds_return_t it_remove_from_list(LDS_ITERATOR *it, void *removed_element) {
    if (it->current == NULL) {
        return LDS_POS_ERR;
    }

    Node * removed = it->current;
    it->current = it->current->next;

    if (it->previous != NULL) {
        it->previous->next = it->current;
    }

    if (it->ds->storage.list.first == removed) {
        it->ds->storage.list.first = it->current;
    }
    if (it->ds->storage.list.last == removed) {
        it->ds->storage.list.last = it->previous;
    }

    if (removed_element != NULL) {
        memcpy(removed_element, removed->data, it->ds->data_size);
    }

    free(removed->data);
    free(removed);
    it->ds->size--;

    return LDS_SUCCESS;
}

static lds_return_t it_reset_in_vector(LDS_ITERATOR *it) {
    it->position = 0;
    return LDS_SUCCESS;
}

static lds_return_t it_reset_in_list(LDS_ITERATOR *it) {
    it->position = 0;
    it->current = it->ds->storage.list.first;
    it->previous = NULL;
    return LDS_SUCCESS;
}

static lds_return_t it_go_in_vector(LDS_ITERATOR *it, size_t position) {
    it->position = position;
    return LDS_SUCCESS;
}

static lds_return_t it_go_in_list(LDS_ITERATOR *it, size_t position) {
    if (position < it->position) {
        it_reset_in_list(it);
    }
    while(it->position < position) {
        it_next_in_list(it);
    }
    return LDS_SUCCESS;
}


static lds_return_t it_set_in_vector(LDS_ITERATOR *it, void *value) {
    return set_element_in_vector(it->ds, it->position, value);
}
static lds_return_t it_set_in_list(LDS_ITERATOR *it, void *value) {
    if (memcmp(value, it->current->data, it->ds->data_size) == 0) {
        return LDS_FAIL;
    }

    memcpy(it->current->data, value, it->ds->data_size);
    return LDS_SUCCESS;
}

/* Funções de pilha */
lds_return_t lds_stack_push(LINEAR_DS * ds, void *value) {
    return lds_insert(ds, 0, value);
}

lds_return_t lds_stack_pop(LINEAR_DS * ds, void *removed_element) {
    return lds_remove(ds, 0, removed_element);
}

lds_return_t lds_stack_peek(LINEAR_DS * ds, void *top) {
    return lds_get(ds, 0, top);
}

/* Funções de fila */
lds_return_t lds_enqueue(LINEAR_DS * ds, void *value) {
    return lds_insert_last(ds, value);
}

lds_return_t lds_dequeue(LINEAR_DS * ds, void *removed_element) {
    return lds_remove(ds, 0, removed_element);
}

lds_return_t lds_queue_front(LINEAR_DS * ds, void *front) {
    return lds_get(ds, 0, front);
}
