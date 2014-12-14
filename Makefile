test_lock:
	gcc -Ofast -std=c99 -o lock_test -lpthread main.c lock_queue.c && time ./lock_test
