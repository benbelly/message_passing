#include "thread_pool.h"

namespace message_passing {

work_queue::work_queue() { }

work_queue::~work_queue() {
    std::lock_guard<std::mutex> lock( queue_mutex );
    while( !queue.empty() ) queue.pop();
}

void work_queue::enqueue( std::unique_ptr<work_item_interface> &work_item ) {
    {
        std::lock_guard<std::mutex> lock( queue_mutex );
        queue.push( std::move( work_item ) );
    }
    queue_signal.notify_all();
}

std::unique_ptr<work_item_interface> work_queue::dequeue() {
    std::unique_lock<std::mutex> lock( queue_mutex );
    auto work_item = get_next();
    if( !work_item ) { // try once to wait for data
        queue_signal.wait( lock );
        work_item = get_next();
    }
    return work_item;
}

/*
 * Try to get the next item from the queue. Returns nullptr if there
 * is nothing available.
 * Does not do locking - only queue access.
 */
work_queue::work_item_interface_ptr work_queue::get_next() {
    work_item_interface_ptr work_item( nullptr );
    if( !queue.empty() ) {
        work_item = std::move( queue.front() );
        queue.pop();
    }
    return work_item;
}

static const int MAX_THREADS = 12;

thread_pool::thread_pool() : thread_pool( MAX_THREADS ) {
}

thread_pool::thread_pool( int thread_count ) :
  max_threads( thread_count ),
  queue( new work_queue() ),
  running( false ) {

    threads.reserve( max_threads );
}

thread_pool::~thread_pool() {
    running = false;
    queue->release();
    for( auto &t : threads ) { t->join(); }
}

/*
 * Problems here with exceptions, if the third thread instantiation throws,
 * we're in a bad way. Clean up?
 */
void thread_pool::start() {
    if( !running ) {
        for( int i = 0; i < max_threads; ++i ) {
            std::unique_ptr<std::thread> new_thread(
                    new std::thread( &thread_pool::run_thread, this ) );
            threads.push_back( std::move( new_thread ) );
        }
        running = true;
    }
}

void thread_pool::run_thread() {
    while( running ) {
        auto work_item = queue->dequeue();
        if( work_item ) work_item->do_work();
    }
}

}

