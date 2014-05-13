#ifndef CONCURRENCY_WORK_QUEUE_
#define CONCURRENCY_WORK_QUEUE_

#include <memory>
#include <functional>

/* TODO - Rename this! Anything would be better */
namespace concurrency {

/*
 * Interface to hold work that needs done.
 */
class work_interface {
    public:
        work_interface();
        virtual ~work_interface() = 0;
        virtual void do_work() = 0;
};

/*
 * Interface to a queue of work to do. This work will be done by the owning threads.
 * You should not be creating an instance of a work_queue_interface child, but get one from the
 * functions below.
 */
class work_queue_interface {
    public:
        work_queue_interface();
        virtual ~work_queue_interface() = 0;
        virtual void enqueue( std::unique_ptr<work_interface> &work ) = 0;
};

/*
 * Get a queue of work to be done asynchronously.
 */
std::shared_ptr<work_queue_interface> get_work_queue();

/*
 * This helper function adds a function to a work_queue_interface.
 * The provided function will be executed asynchronously.
 */
void do_work( std::shared_ptr<work_queue_interface> &queue,
              const std::function<void ()> &work_func );

}

#endif
