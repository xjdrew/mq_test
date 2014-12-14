all:test_lock test_cas

test_lock:
	gcc -Ofast -std=c99 -pthread -w -DLOCK_QUEUE -o lock_test main.c lock_queue.c && echo "+++++ test lock +++++" && time ./lock_test

test_cas:
	gcc -Ofast -std=c99 -pthread -w -o cas_test main.c cas_queue.c && echo "+++++ cas lock +++++" && time ./cas_test
