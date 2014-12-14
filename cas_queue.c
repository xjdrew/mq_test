#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "cas_queue.h"

#define MAX_MQ_LEN 0x10000
#define GP(p) ((p) % MAX_MQ_LEN)

struct queue {
    uint32_t head;
    uint32_t tail;
    struct message **msgs;
    bool *flag;
};

struct queue *Q;
void qinit() {
    struct queue *q = calloc(1, sizeof(*q));
    q->msgs = calloc(MAX_MQ_LEN, sizeof(struct message*));
    q->flag = calloc(MAX_MQ_LEN, sizeof(bool));
    Q = q;
}

void qpush(struct message *m) {
    struct queue *q= Q;

    uint32_t tail = GP(__sync_fetch_and_add(&q->tail,1));
    // The thread would suspend here, and the q->msgs[tail] indexs last version ,
    // but the msg tail is increased.
    // So we set q->flag[tail] after changing q->msgs[tail].
    q->msgs[tail] = m;
    q->flag[tail] = true;
    __sync_synchronize();
}


struct message *
qpop() {
    struct queue *q = Q;
    uint32_t head =  q->head;
    uint32_t head_ptr = GP(head);
    if (head_ptr == GP(q->tail)) {
        return NULL;
    }

    // Check the flag first, if the flag is false, the pushing may not complete.
    if(!q->flag[head_ptr]) {
        return NULL;
    }
    
    struct message *m = q->msgs[head_ptr];
    if (!__sync_bool_compare_and_swap(&q->head, head, head+1)) {
        return NULL;
    }
    q->flag[head_ptr] = false;
    return m;
}

