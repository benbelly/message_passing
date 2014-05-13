
#include "test_thread_pool.h"
#include "thread_pool.h"
#include<iostream>
#include<thread>
#include<chrono>

void pause( int duration ) {
    std::this_thread::sleep_for( std::chrono::milliseconds( duration ) );
}

bool basic_async_work() {
    auto q = concurrency::get_work_queue();
    bool success = false;
    concurrency::do_work( q, [&]() { success = true; } );
    pause( 100 );
    return success;
}

int main() { 
    bool result = basic_async_work();
    return result ? 0 : -1;
}
