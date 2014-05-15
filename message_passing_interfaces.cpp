
#include "message_passing_interfaces.h"
#include "thread_pool.h"

namespace message_passing {

using std::shared_ptr;
using std::unique_ptr;
using std::function;

work_item_interface::work_item_interface() { }
work_item_interface::~work_item_interface() {}

work_queue_interface::work_queue_interface() {}
work_queue_interface::~work_queue_interface() {}

shared_ptr<work_queue_interface> get_work_queue() {
    static thread_pool pool;
    pool.start();
    return pool.get_work_queue();
}

shared_ptr<work_queue_interface> get_work_queue( int num_concurrent_items) {
    static thread_pool pool( num_concurrent_items );
    pool.start();
    return pool.get_work_queue();
}

struct work_item_function : public work_item_interface {
        work_item_function( const function<void ()> &f ) : work( f ) {}
        virtual ~work_item_function() {}
        virtual void do_work() { work(); }
        function<void ()> work;
};

void do_work( shared_ptr<work_queue_interface> &queue,
              const function<void ()> &work_function ) {
    unique_ptr<work_item_interface> work_item_ptr(
            new work_item_function( work_function ) );
    queue->enqueue( work_item_ptr );
}

}
