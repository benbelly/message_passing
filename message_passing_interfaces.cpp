
#include "message_passing_interfaces.h"
#include "thread_pool.h"

namespace message_passing {

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::function;

work_item_interface::work_item_interface() {}
work_item_interface::~work_item_interface() {}

work_queue_interface::work_queue_interface() {}
work_queue_interface::~work_queue_interface() {}


shared_ptr<work_queue_interface> get_work_queue( int num_concurrent_items) {
    struct queue_wrapper : public work_queue_interface {
        virtual ~queue_wrapper() {}
        shared_ptr<thread_pool> thread_pool;
        shared_ptr<work_queue_interface> work_queue;
        virtual void enqueue( unique_ptr<work_item_interface> &work ) final {
            work_queue->enqueue( work );
        }
    };
    auto work_queue = make_shared<queue_wrapper>();
    work_queue->thread_pool =
               make_shared<thread_pool>( num_concurrent_items );
    work_queue->work_queue = work_queue->thread_pool->get_work_queue();
    work_queue->thread_pool->start();
    return work_queue;
}


void add_work( shared_ptr<work_queue_interface> &queue,
               const function<void ()> &work_function ) {
    struct work_item_function : public work_item_interface {
        work_item_function( const function<void ()> &f ) : work( f ) {}
        virtual ~work_item_function() {}
        virtual void do_work() { work(); }
        function<void ()> work;
    };
    unique_ptr<work_item_interface> work_item_ptr =
                            make_unique<work_item_function>( work_function );
    queue->enqueue( work_item_ptr );
}

}
