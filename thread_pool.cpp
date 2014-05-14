#include "thread_pool.h"
#include <iostream>

namespace message_passing {

work_queue::~work_queue() {
    // Lock just to be nice. Shouldn't be contended...
    std::lock_guard<std::mutex> lock( queue_mutex );
    while( !queue.empty() ) queue.pop();
}

void work_queue::enqueue( std::unique_ptr<work_interface> &work_item ) {
    {
        std::lock_guard<std::mutex> lock( queue_mutex );
        queue.push( std::move( work_item ) );
    }
    queue_signal.notify_all();
}

std::unique_ptr<work_interface> work_queue::dequeue() {
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
work_queue::work_interface_ptr work_queue::get_next() {
    work_interface_ptr work_item( nullptr );
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

void thread_pool::start() {
    std::lock_guard<std::mutex> lock( thread_lock );
    if( !running ) {
        std::unique_ptr<std::thread> new_thread(
                new std::thread( [this]() { this->run_thread(); } ) );
        threads.push_back( std::move( new_thread ) );
        running = true;
    }
}

void thread_pool::run_thread() {
    while( running ) {
        auto work_item = queue->dequeue();
        if( work_item ) work_item->do_work();
        //else std::cout << "That's weird\n";
    }
}

}

