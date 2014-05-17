#ifndef MESSAGE_PASSING_WORK_QUEUE_
#define MESSAGE_PASSING_WORK_QUEUE_

#include <memory>
#include <functional>

namespace message_passing {

/*
 * Interface to hold work that needs done.
 */
class work_item_interface {
    public:
        work_item_interface();
        virtual ~work_item_interface() = 0;
        virtual void do_work() = 0;
};

/*
 * work_queue_interface is an interface to a queue of work to do. This work
 * will be done by the owning threads. You should not be creating an instance
 * of a work_queue_interface child, but get one from the functions below.
 */
class work_queue_interface {
    public:
        work_queue_interface();
        virtual ~work_queue_interface() = 0;
        virtual void enqueue( std::unique_ptr<work_item_interface> &work ) = 0;
};

/*
 * Get a queue of work to be done asynchronously.
 */
std::shared_ptr<work_queue_interface> get_work_queue();

/*
 * Get a queue of work to be done asynchronously, with a maximum of
 * 'concurrent_items' executing simultaneously.
 */
std::shared_ptr<work_queue_interface> get_work_queue( int concurrent_items );

/*
 * Get a queue of work to be done asynchronously from the enqueueing code, but
 * serially - that is, each work_item will complete before the next is started.
 */
std::shared_ptr<work_queue_interface> get_serialized_queue();

/*
 * Add a function to a work_queue_interface.
 * The provided function will be executed asynchronously.
 */
void do_work( std::shared_ptr<work_queue_interface> &queue,
              const std::function<void ()> &work_func );

}

#endif
