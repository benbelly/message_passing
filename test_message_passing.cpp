#include "test_message_passing.h"
#include "message_passing_interfaces.h"
#include<iostream>
#include<thread>
#include<chrono>
#include<vector>

typedef std::lock_guard<std::mutex> lock_guard;

void pause( int duration_milliseconds ) {
    std::this_thread::sleep_for( 
            std::chrono::milliseconds( duration_milliseconds ) );
}

bool basic_async_work() {
    bool success = false;
    auto q = message_passing::get_work_queue();
    message_passing::do_work( q, [&]() { success = true; } );
    pause( 10 );
    return success;
}

bool basic_pooled_work() {
    bool success = false;
    auto q = message_passing::get_work_queue( 3 );

    /* Start one, then two, then three, then four.
     * The first three finish in reverse order, followed by the fourth */
    std::vector<int> expected = { 3, 2, 1, 4 }, fs;
    fs.reserve( 4 ); // Probably not needed, but I don't want to realloc

    std::mutex m;

    message_passing::do_work( q, [&m, &fs]() { pause( 500 );
                                               lock_guard guard( m );
                                               fs.push_back( 1 ); } );
    message_passing::do_work( q, [&m, &fs]() { pause( 300 );
                                               lock_guard guard( m );
                                               fs.push_back( 2 ); } );
    message_passing::do_work( q, [&m, &fs]() { pause( 100 );
                                               lock_guard guard( m );
                                               fs.push_back( 3 ); } );
    message_passing::do_work( q, [&m, &fs]() { pause( 410 );
                                               lock_guard guard( m );
                                               fs.push_back( 4 ); } );

    pause( 800 );

    success = expected == fs;

    return success;
}

int main() { 
    typedef std::function<bool ()> test_func;

    test_func tests[] = { basic_async_work, basic_pooled_work };

    int tests_count = sizeof( tests ) / sizeof( test_func );
    test_func *tests_end = tests + tests_count;
    int passed = std::count_if( tests, tests_end,
                                [](test_func f){ return f(); } );

    std::cout << passed << " passed of " << tests_count << " run.\n";
    return passed == tests_count ? 0 : tests_count - passed;
}

