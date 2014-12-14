#include <stdlib.h>
#include <assert.h>

#include "lock_queue.h"

struct queue {
    struct message *head;
    struct message *tail;
    int lock;
};

#define LOCK(q) while (__sync_lock_test_and_set(&(q)->lock,1)) {}
#define UNLOCK(q) __sync_lock_release(&(q)->lock);

struct queue *Q;

void qinit() {
    struct queue *q = calloc(1, sizeof(*q));
    Q = q;
}

void qpush(struct message *m) {
    struct queue *q= Q;

    LOCK(q)
    assert(m->next == NULL);
    if(q->tail) {
        q->tail->next = m;
        q->tail = m;
    } else {
        q->head = q->tail = m;
    }
    UNLOCK(q)
}

struct message *
qpop() {
    struct queue *q = Q;
        
    LOCK(q)
    struct message *head = q->head;
    if(head) {
        q->head = head->next;
        if(q->head == NULL) {
            assert(head == q->tail);
            q->tail = NULL;
        }
        head->next = NULL;
    }
    UNLOCK(q)

    return head;
}


