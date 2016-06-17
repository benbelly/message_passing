#include "test_message_passing.h"
#include "message_passing_interfaces.h"
#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<mutex>
#include<algorithm>

typedef std::lock_guard<std::mutex> lock_guard;
using std::vector;
using std::mutex;

void pause( int duration_milliseconds ) {
    std::this_thread::sleep_for( 
            std::chrono::milliseconds( duration_milliseconds ) );
}

bool basic_async_work() {
    bool success = false;
    auto q = message_passing::get_work_queue();
    message_passing::add_work( q, [&]() { success = true; } );
    pause( 10 );
    return success;
}

bool basic_pooled_work() {
    auto q = message_passing::get_work_queue( 3 );

    /* Start one, then two, then three, then four.
     * The first three finish in reverse order, followed by the fourth */
    vector<int> expected = { 3, 2, 1, 4 }, fs;
    fs.reserve( 4 );

    message_passing::add_work( q, [&fs]() { pause( 500 );
                                            fs.push_back( 1 ); } );
    message_passing::add_work( q, [&fs]() { pause( 300 );
                                            fs.push_back( 2 ); } );
    message_passing::add_work( q, [&fs]() { pause( 100 );
                                            fs.push_back( 3 ); } );
    message_passing::add_work( q, [&fs]() { pause( 410 );
                                            fs.push_back( 4 ); } );

    pause( 600 );

    bool success = expected == fs;

    return success;
}

bool basic_serialized_queue() {
    auto q = message_passing::get_serialized_queue();

    /* Start one, then two, then three
     * They finish in order, because the queue is serialized, executing each
     * item in the order it is received.
     */
    vector<int> expected = { 1, 2, 3 }, fs;
    fs.reserve( 3 );

    message_passing::add_work( q, [&fs]() { pause( 50 );
                                            fs.push_back( 1 ); } );
    message_passing::add_work( q, [&fs]() { pause( 30 );
                                            fs.push_back( 2 ); } );
    message_passing::add_work( q, [&fs]() { pause( 10 );
                                            fs.push_back( 3 ); } );

    pause( 120 );

    bool success = expected == fs;

    return success;
}

int main() { 
    typedef std::function<bool ()> test_func;

    test_func tests[] = { basic_async_work, basic_pooled_work,
                          basic_serialized_queue };

    int tests_count = sizeof( tests ) / sizeof( test_func );
    test_func *tests_end = tests + tests_count;
    int passed = std::count_if( tests, tests_end,
                                [](test_func f){ return f(); } );

    std::cout << passed << " passed of " << tests_count << " run.\n";
    return tests_count - passed;
}

