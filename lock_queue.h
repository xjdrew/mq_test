#ifndef LOCK_QUEUE_H
#define LOCK_QUEUE_H

struct message {
    int v;
    struct message *next;
};

void qinit();
void qpush(struct message *m);
struct message* qpop();
#endif

