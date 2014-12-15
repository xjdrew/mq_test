#CFLAGS=-Ofast -std=c99 -pthread -w 
CFLAGS=-O0 -g -std=c99 -pthread -w 
all:lock_queue cas_array taa_queue
test:test_lock_queue test_cas_array test_taa_queue

lock_queue:
	gcc $(CFLAGS) -DLOCK_QUEUE -o lock_test main.c lock_queue.c 

test_lock_queue: lock_queue
	time ./lock_test

cas_array:
	gcc $(CFLAGS) -DCAS_ARRAY -o casa_test main.c cas_array.c 

test_cas_array: cas_array
	 time ./casa_test

taa_queue:
	gcc $(CFLAGS) -DTAA_QUEUE -o taa_test main.c taa_queue.c 

test_taa_queue: taa_queue
	time ./taa_test

# donot finish
# test_cas_queue:
# 	gcc -Ofast -std=c99 -pthread -w -o casq_test main.c cas_queue.c && echo "+++++ test cas queue +++++" && time ./casq_test
