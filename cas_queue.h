#ifndef CAS_QUEUE_H
#define CAS_QUEUE_H
struct message {
    void *p;
};
void qinit();
void qpush(struct message *m);
struct message* qpop();
#endif
