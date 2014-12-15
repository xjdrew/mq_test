#include <stdlib.h>
#include <assert.h>

#include "lock_queue.h"

/*
#define INVALID_MEM_ADDR ((void*)-1)
struct queue {
    struct message *head;
    struct message *tail;
    int lock;
};

struct queue *Q;

void qinit() {
    struct queue *q = calloc(1, sizeof(*q));
    Q = q;
}

void qpush(struct message *m) {
    assert(m->next == NULL);

    struct queue *q= Q;
    struct message* tail;
    do {
        tail = q->tail;
        if(tail) {
            tail->next = INVALID_MEM_ADDR;
        }
        if(!__sync_bool_compare_and_swap(&q->tail, tail, m)) {
            continue;
        }
        if(tail) {
            if(!__sync_bool_compare_and_swap(&tail->next, INVALID_MEM_ADDR, m)) {
                // tail is poped
                assert(q->head == NULL || q->head == INVALID_MEM_ADDR);
                q->head = m;
            }
        } else {
            // queue is empty
            assert(q->head == NULL);
            q->head = m;
        }
        __sync_synchronize();
        break;
    } while(1);
}

struct message *
qpop() {
    struct queue *q = Q;
    struct message *head;
    do {
        head = q->head;
        if(!head) {
            break;
        }
        if(__sync_bool_compare_and_swap(&q->head, head, head->next)) {
            __sync_bool_compare_and_swap(&q->head, INVALID_MEM_ADDR, q->tail);
            __sync_bool_compare_and_swap(&q->tail, head, NULL);
            head->next = NULL;
            break;
        }
    } while(1);

    return head;
}
*/
