#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "pthread_lock_queue.h"

struct queue {
    struct message *head;
    struct message *tail;
    pthread_mutex_t lock;
};

#define LOCK(q) pthread_mutex_lock(&q->lock);
#define UNLOCK(q) pthread_mutex_unlock(&q->lock);

struct queue *Q;

void qinit() {
    struct queue *q = calloc(1, sizeof(*q));
    pthread_mutex_init(&q->lock, NULL);
    Q = q;
}

void qpush(struct message *m) {
    struct queue *q= Q;

    LOCK(q)
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
            q->tail = NULL;
        }
        head->next = NULL;
    }
    UNLOCK(q)

    return head;
}



