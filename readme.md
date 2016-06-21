
An Example Message Passing Library
=

The goal of this library to provide an example of a concurrent message passing
system in order to aid discussion and to show how to encapsulate the some
thread primitives behind a small set of classes or functions to create a
scalable, message-passing concurrency model that supports parallelization and
asynchronous protocols between actors.

This is *not* to a cross-platform, general use, robust library. It is just an example for discussion.

If you have suggestions around exception safety, I'm happy to hear them.

Building:
-
I built this on a Macbook Pro running OSX Mavericks using clang 3.4. I have
built and tested this on Ubuntu 14.04, also with clang 3.4. Everything is
standard C++, so the code should build fine on Windows with Visual Studio 2013,
or later, but I haven't tested that.

Run `make`, or `make -f Makefile` to build and run the test. Output goes to the
`build/lib` and `build/include` directories.

How to use:
-
1. `#include"message_passing_interfaces.h"`
2. Link against libmessagepassing.a
3. Use `get_work_queue()` to get a work queue.
4. Call `add_work( queue, std::function )` to call that function asynchronously.

Requirements:
-
* Use as few third-party libraries as possible. (C++ standard library is fine.)
* Use standard C++ for portability.
* Clients should not need to use any threading primitives directly.
* In many cases, degree of parallelization (e.g. how many threads could be
  supported) is a domain consideration, not a library concern. This number
  should be provided by client code.
* Single-thread queues must be possible in order to support an Active Object
  pattern.

To Do:
-
* Create an example of actors passing messages using these classes.
* Examine exception safety. Even examples can improve on this.
* Support 'tagged' queues so that clients can get a particular work queue.
* Thread_pool running state management is ugly. Clean it up.
* Currently all threads in a thread-pool are created at once. I am interested
  in delaying thread creation until there is work for the thread to do. Again,
  this may be more fun than valuable to an example library.
