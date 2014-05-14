
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
    bool success = false;
    auto q = message_passing::get_work_queue();
    message_passing::do_work( q, [&]() { success = true; } );
    //message_passing::do_work( q, [&]() { success = true; } );
    //message_passing::do_work( q, [&]() { success = true; } );
    pause( 100 );
    return success;
}

bool basic_pooled_work() {
    bool success = false;
    //auto q = message_passing::get_work_queue( 3 );
    return success;
}

int main() { 
    typedef std::function<bool ()> test_func;

    test_func tests[] = { basic_async_work, /*basic_pooled_work*/ };
    int tests_size = sizeof( tests ) / sizeof( test_func );

    bool result = std::all_of( tests, tests + tests_size, [](test_func f){ return f(); } );

    std::string result_str = result ? "Success!" : "Failure.";
    std::cout << "Result of " << tests_size
              << " tests is " << result_str << std::endl;
    return result ? 0 : 10;
}
