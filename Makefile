all:test_lock test_cas

test_lock:
	gcc -Ofast -std=c99 -w -DLOCK_QUEUE -o lock_test -lpthread main.c lock_queue.c && echo "+++++ test lock +++++" && time ./lock_test

test_cas:
	gcc -Ofast -std=c99 -w -o cas_test -lpthread main.c cas_queue.c && echo "+++++ cas lock +++++" && time ./cas_test
