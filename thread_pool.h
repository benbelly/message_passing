#ifndef MESSAGE_PASSING_THREAD_POOL_
#define MESSAGE_PASSING_THREAD_POOL_

#include "message_passing_interfaces.h"
#include<thread>
#include<mutex>
#include<queue>

namespace message_passing {

class work_queue : public work_queue_interface {
    public:
        virtual ~work_queue();
        virtual void enqueue( std::unique_ptr<work_item_interface> &work_item );

    private:
        friend class thread_pool;
        typedef std::unique_ptr<work_item_interface> work_interface_ptr;

        work_interface_ptr dequeue();
        void release() { queue_signal.notify_all(); }

        work_interface_ptr get_next();

        std::mutex queue_mutex;
        std::condition_variable queue_signal;
        std::queue<work_interface_ptr> queue;
};

class thread_pool {
    public:
        thread_pool();
        thread_pool( int thread_count );
        virtual ~thread_pool();

        void start();

        std::shared_ptr<work_queue_interface> get_work_queue() const {
            return queue;
        }

    private:
        int max_threads;
        std::mutex thread_lock;
        std::vector<std::unique_ptr<std::thread>> threads;
        std::shared_ptr<work_queue> queue;

        volatile bool running;

        void run_thread();
};

}

#endif
