#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <pthread.h>

#ifdef LOCK_QUEUE
#include "lock_queue.h"
#elif CAS_ARRAY
#include "cas_array.h"
#elif TAA_QUEUE
#include "taa_queue.h"
#else
#include "cas_queue.h"
#endif

#define WORKERS 10
#define MESSAGES 700 * 10000

struct message *msgs;
int *checker;

void* worker(void *arg) {
    int index = (int)arg;
    int count = (MESSAGES/WORKERS);
    int offset = count * index;
    int start = offset;
    int end = count + offset;

    int pushed = 0;
    int poped = 0;
    struct message *msg;
    for (int i=start; i< end; i++) {
        qpush(&msgs[i]);
        ++pushed;
        do {
            msg = qpop();
            if(!msg) {
                continue;
            }
            __sync_fetch_and_add(&checker[msg->v], 1);
            ++poped;
        } while(!msg);
    }
    printf("thread %d finish,push %d msgs, pop %d msgs\n", index, pushed, poped);
}

int main() {
    qinit();

    pthread_t threads[WORKERS];
    int count = (MESSAGES/WORKERS);
    int total = count * WORKERS;
    msgs = calloc(total, sizeof(struct message));
    checker = calloc(total, sizeof(int));

    // set msg value
    for(int i=0; i<total; i++) {
        msgs[i].v = i;
    }

    for (int i=0; i<WORKERS; i++) {
        assert(pthread_create(&threads[i], NULL, worker, (void*)i)==0);
    }
    for (int i=0; i<WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // check pop
    for(int i=0; i<total; i++) {
        assert(checker[i]==1);
    }
    return 0;
}

