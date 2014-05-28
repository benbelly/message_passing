## Set the compiler flags
CPP=clang++
# CPP=g++ # On linux
CPPFLAGS=
DEBUG=-g
WARN=-Wall -Werror -Wextra
STD=-std=c++11
AR=ar
AR_ARGS=rvs

all: test

test: output libmessagepassing.a test_message_passing.cpp test_message_passing.h
	$(CPP) $(CPPFLAGS) $(DEBUG) $(WARN) $(STD) -Lbuild/lib -lmessagepassing test_message_passing.cpp -o test_message_passing
	./test_message_passing

libmessagepassing.a: output thread_pool.o message_passing_interfaces.o
	$(AR) $(AR_ARGS) build/lib/libmessagepassing.a build/object/thread_pool.o build/object/message_passing_interfaces.o

thread_pool.o: output message_passing_interfaces.o thread_pool.cpp thread_pool.h
	$(CPP) $(CPPFLAGS) $(DEBUG) $(WARN) $(STD) thread_pool.cpp -c -o build/object/thread_pool.o

message_passing_interfaces.o: output message_passing_interfaces.cpp message_passing_interfaces.h
	$(CPP) $(CPPFLAGS) $(DEBUG) $(WARN) $(STD) message_passing_interfaces.cpp -c -o build/object/message_passing_interfaces.o

output:
	mkdir -p build/lib build/include build/object
	cp message_passing_interfaces.h build/include

clean:
	rm -f thread_pool.o message_passing_interfaces.o test_message_passing libmessagepassing.a
	rm -rf test_message_passing.dSYM/
	rm -rf build

