all:test_lock_queue test_cas_array

test_lock_queue:
	gcc -Ofast -std=c99 -pthread -w -DLOCK_QUEUE -o lock_test main.c lock_queue.c && echo "+++++ test lock queue +++++" && time ./lock_test

test_cas_array:
	gcc -Ofast -std=c99 -pthread -w -DCAS_ARRAY -o casa_test main.c cas_queue.c && echo "+++++ test cas array +++++" && time ./casa_test

# donot finish
# test_cas_queue:
# 	gcc -Ofast -std=c99 -pthread -w -o casq_test main.c cas_queue.c && echo "+++++ test cas queue +++++" && time ./casq_test
