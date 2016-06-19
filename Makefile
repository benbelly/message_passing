## Set the compiler flags
CPP=clang++
CPP_FLAGS=-pthread
DEBUG=-g
WARN=-Wall -Wextra -Werror
STD=-std=c++11
OPT=-O3
AR=ar
AR_ARGS=rcvs

all: test

test: output libmessagepassing.a test_message_passing.cpp test_message_passing.h
	$(CPP) $(CPP_FLAGS) $(DEBUG) $(OPT) $(WARN) $(STD) test_message_passing.cpp -Lbuild/lib -lmessagepassing -o test_message_passing
	./test_message_passing

libmessagepassing.a: output thread_pool.o message_passing_interfaces.o
	$(AR) $(AR_ARGS) build/lib/libmessagepassing.a build/object/message_passing_interfaces.o build/object/thread_pool.o

thread_pool.o: output message_passing_interfaces.o thread_pool.cpp thread_pool.h
	$(CPP) $(CPP_FLAGS) $(DEBUG) $(WARN) $(STD) thread_pool.cpp -c -o build/object/thread_pool.o

message_passing_interfaces.o: output message_passing_interfaces.cpp message_passing_interfaces.h
	$(CPP) $(CPP_FLAGS) $(DEBUG) $(OPT) $(WARN) $(STD) message_passing_interfaces.cpp -c -o build/object/message_passing_interfaces.o

output:
	mkdir -p build/lib build/include build/object
	cp message_passing_interfaces.h build/include

clean:
	rm -f thread_pool.o message_passing_interfaces.o test_message_passing libmessagepassing.a
	rm -rf test_message_passing.dSYM/
	rm -rf build

