
#include "test_message_passing.h"
#include "work_queue.h"
#include<iostream>
#include<thread>
#include<chrono>

void pause( int duration_milliseconds ) {
    std::this_thread::sleep_for( 
            std::chrono::milliseconds( duration_milliseconds ) );
}

bool basic_async_work() {
    auto q = message_passing::get_work_queue();
    bool success = false;
    message_passing::do_work( q, [&]() { success = true; } );
    pause( 100 );
    return success;
}

int main() { 
    bool result = basic_async_work();
    return result ? 0 : -1;
}
