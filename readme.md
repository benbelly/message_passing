
An Example Message Passing Library
=

I wrote this library to provide an example of a message passing system that
supports concurrency.

Direct use of threading primitives (mutexes, locks, condition variables,
threads) to manage concurrency does not work well for a team of more than one.
Even a single developer will find that taking a break from a project for few
weeks will require re-learning the concurrency rules for a system built on
primitives. Nevertheless, I see a lot of code like this. The goal of this
library is to demonstrate how one can encapsulate the primitives behind a
small set of classes or functions to create a scalable, message-passing
concurrency model that supports asynchronous protocols between actors.

The goal is *not* to provide a cross-platform, every possible use, robust for
every occasion library. It is an example to build from and support
discussion.

*A note on exceptions.* C++ has a problem with exceptions. They are just
broken. I respect Herb Sutter, and appreciate everything he has done, but it
bothers me that a person can write two books and make a living explaining how
to write correct C++ exception-safe code. The language is broken, and I
advise you to just turn exceptions off. Nevertheless, if you have suggestions
around exception safety, I'm happy to hear them.

Requirements:
-
* Use as few third-party libraries as possible. (C++ standard library is fine.)
* Use standard C++ for portability.
* Clients should not need to use any threading primitives directly.
* In many cases, degree of parallelization (e.g. how many threads could be
  supported) is a domain consideration, not a generic library concern. This
  number should be provided by client code when desired.
* Serialization of messages is required in order to support an
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
