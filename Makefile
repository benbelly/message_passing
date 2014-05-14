## Set the compiler flags
CPP=clang++
DEBUG=-g
WARN=-Wall -Werror -Wextra
STD=-std=c++11

all: test

test: thread_pool.o work_queue.o test_message_passing.cpp test_message_passing.h
	$(CPP) $(DEBUG) $(WARN) $(STD) thread_pool.o work_queue.o test_message_passing.cpp -o test_message_passing

thread_pool.o: work_queue.o thread_pool.cpp thread_pool.h
	$(CPP) $(DEBUG) $(WARN) $(STD) thread_pool.cpp -c -o thread_pool.o

work_queue.o: work_queue.cpp work_queue.h
	$(CPP) $(DEBUG) $(WARN) $(STD) work_queue.cpp -c -o work_queue.o

clean: thread_pool.o work_queue.o test_message_passing
	rm -f thread_pool.o work_queue.o test_message_passing

