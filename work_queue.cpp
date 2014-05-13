
#include "work_queue.h"
#include "thread_pool.h"

namespace concurrency {

work_interface::work_interface() { }
work_interface::~work_interface() {}

work_queue_interface::work_queue_interface() {}
work_queue_interface::~work_queue_interface() {}

std::shared_ptr<work_queue_interface> get_work_queue() {
    static thread_pool pool;
    pool.start();
    return pool.get_work_queue();
}

struct function_work : work_interface {
        function_work( const std::function<void ()> &f ) : work( f ) {}
        virtual ~function_work() {}
        virtual void do_work() { work(); }
        std::function<void ()> work;
};

void do_work( std::shared_ptr<work_queue_interface> &queue,
              const std::function<void ()> &work_func ) {
    std::unique_ptr<work_interface> work_ptr( new function_work( work_func ) );
    queue->enqueue( work_ptr );
}

}
