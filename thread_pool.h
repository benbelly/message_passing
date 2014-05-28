#ifndef MESSAGE_PASSING_THREAD_POOL_
#define MESSAGE_PASSING_THREAD_POOL_

#include "message_passing_interfaces.h"
#include<thread>
#include<condition_variable>
#include<mutex>
#include<queue>

namespace message_passing {

class work_queue : public work_queue_interface {
    public:
        virtual ~work_queue();
        virtual void enqueue( std::unique_ptr<work_item_interface> &work_item );

    private:
        friend class thread_pool;
        work_queue();

        typedef std::unique_ptr<work_item_interface> work_item_interface_ptr;

        work_item_interface_ptr dequeue();
        void release();

        work_item_interface_ptr get_next();

        std::mutex queue_mutex;
        std::condition_variable queue_signal;
        std::queue<work_item_interface_ptr> queue;
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
        std::vector<std::unique_ptr<std::thread>> threads;
        std::shared_ptr<work_queue> queue;

        void run_thread();

        bool is_running();
        void set_running( bool now_running );
        bool running;
        std::mutex running_mutex;
};

}

#endif
