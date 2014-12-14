#include <stdio.h>
#include <assert.h>

#include <pthread.h>

#ifdef LOCK_QUEUE
#include "lock_queue.h"
#else
#include "cas_queue.h"
#endif

#define WORKERS 2
#define MESSAGES 700 * 10000

struct message *msgs;

void* worker(void *arg) {
    int index = (int)arg;
    int count = (MESSAGES/WORKERS);
    int offset = count * index;
    int start = offset;
    int end = count + offset;
    for (int i=start; i< end; i++) {
        qpush(&msgs[i]);
        while(qpop()) {
            break;
        }
    }
    printf("thread %d finish %d msgs\n", index, count);
}

int main() {
    qinit();

    pthread_t threads[WORKERS];
    int count = (MESSAGES/WORKERS);
    msgs = calloc(count * MESSAGES, sizeof(struct message));

    for (int i=0; i<WORKERS; i++) {
        assert(pthread_create(&threads[i], NULL, worker, (void*)i)==0);
    }
    for (int i=0; i<WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

