
An Example Message Passing Library
=

I wrote this library to provide an example of a concurrent message passing
system in order to aid discussion.

Direct use of threading primitives (mutexes, locks, condition variables,
threads) to manage concurrency does not work well for a team of more than one.
Even a single developer will find that taking a break from a project for few
weeks will require re-learning the concurrency rules for a system built on
primitives. Nevertheless, I see a lot of code like this. The goal of this
library is to demonstrate how one can encapsulate the primitives behind a
small set of classes or functions to create a scalable, message-passing
concurrency model that supports parallelization and asynchronous protocols
between actors.

The goal is *not* to provide a cross-platform, every possible use, robust for
every occasion library. It is an example to build from and support
discussion.

*A note on exceptions.* C++ has a problem with exceptions. They are just
broken. I respect Herb Sutter, and appreciate everything he has done, but it
bothers me that a person can write two books and make a living explaining how
to write correct C++ exception-safe code. The language is broken, and I
advise you to just turn exceptions off. Nevertheless, if you have suggestions
around exception safety, I'm happy to hear them.

Building:
-
I built this on a Macbook Pro running OSX Mavericks using clang 3.4. I have
built and tested this on Ubuntu 14.04, also with clang 3.4. Everything is
standard C++, so the code should build fine on Windows with Visual Studio 2013,
or later, but I haven't tested that.

Anyway, just run `make`, or `make -f Makefile` to build and run the test.
Output goes to the `build/lib` and `build/include` directories.

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
  supported) is a domain consideration, not a generic library concern. This
  number should be provided by client code when desired.
* Serialization of messages must be possible in order to support an
  Active Object pattern.


To Do:
-
* Create an example of actors passing messages using these classes.
* Support multiple serial queues.
* Support multiple sets of parallel queues.
* Examine exception safety. Even examples can improve on this.
* Support 'tagged' queues so that clients can get a particular work queue.
* Thread_pool running state management is ugly. Clean it up.
* I have been challenged to use a single thread pool to support both parallel
  and serial queues. I may do this, since it sounds interesting. More fun than
  useful, I suppose.
* Currently all threads in a thread-pool are created at once. I am interested
  in delaying thread creation until there is work for the thread to do. Again,
  this may be more fun than useful.
