#include <stdio.h>
#include <assert.h>

#include <pthread.h>

#include "lock_queue.h"

#define WORKERS 2
#define MESSAGES 700 * 10000

void* worker(void *arg) {
    int count = (int) arg;
    struct message *m = calloc(1, sizeof(*m));
    for (int i=0; i< count; i++) {
        qpush(m);
        assert(qpop() != NULL);
    }
    printf("%d finished\n", count);
}

int main() {
    qinit();

    pthread_t threads[WORKERS];
    int count = (MESSAGES/WORKERS);

    for (int i=0; i<WORKERS; i++) {
        assert(pthread_create(&threads[i], NULL, worker, (void*)count)==0);
    }
    for (int i=0; i<WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

