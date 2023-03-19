//
// Created by Joel Sprunger on 3/17/23.
//

#ifndef CACHE_SCOPEDTHREAD_H
#define CACHE_SCOPEDTHREAD_H
#include <thread>
#include <iostream>

using namespace std;

class ScopedThread{
    thread t;
public:
    explicit ScopedThread(thread t_): t(move(t_)){
        if ( !t.joinable()) throw logic_error("No thread");
    }
    ~ScopedThread(){
        t.join();
    }
    ScopedThread(ScopedThread&)= delete;
    ScopedThread& operator=(ScopedThread const &)= delete;
};


#endif //CACHE_SCOPEDTHREAD_H
