#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "taa_queue.h"

#define taa(m) (__sync_lock_test_and_set(&(m)->lock,1) == 0)
#define utaa(m) __sync_lock_release(&(m)->lock)

#define cas(ptr, oldvar, newval) __sync_bool_compare_and_swap(ptr, oldvar, newval)

struct queue {
    struct message *head;
    struct message *tail;
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
    bool ok = false;
    do {
        tail = q->tail;
        if(tail == NULL) {
            if((ok = cas(&q->tail, tail, m))) {
                assert(q->head == NULL);
                q->head = m;
            }
        } else {
            // avoid pop tail
            if(taa(tail)) {
                if((ok = cas(&q->tail, tail, m))) {
                    tail->next = m;
                }
                utaa(tail);
            }
        }
    } while(!ok);
}

struct message *
qpop() {
    struct queue *q = Q;
    struct message *head;
    bool ok = false;
    do {
        head = q->head;
        if(head == NULL) {
            break;
        }

        if(head->next != NULL) {
            if((ok = cas(&q->head, head, head->next))) {
                head->next = NULL;
            }
        } else {
            if(taa(head)) {
                if((ok = cas(&q->head, head, head->next))) {
                    cas(&q->tail, head, NULL);
                    head->next = NULL;
                }
                utaa(head);
            }
        }
    } while(!ok);
    return head;
}
