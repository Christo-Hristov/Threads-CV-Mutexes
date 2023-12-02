
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "stack.hh"
#include "thread.hh"
#include "timer.hh"

using namespace std;

// This statement is necessary to allocate memory space for the class
// static variable ready_ and initialize it (the declaration in thread.hh
// doesn't actually create space for the variable, just like declaring
// a method in a header file doesn't create code for the method).
queue<Thread*> Thread::ready_;

Thread* Thread::cur_thread = nullptr;
Thread* Thread::last_deleted = nullptr;

void
Thread::thread_start(Thread *t)
{
    t->my_func();
    while (1) {}
}

Thread::Thread(function<void()> main) : my_func(main)
{
    cur_stack = new Stack(Thread::thread_start, this);
    Thread::ready_.push(this);
}

Thread *
Thread::current()
{
    return Thread::cur_thread;
}

void
Thread::schedule()
{
    Thread::ready_.push(this);
    
}

void
Thread::redispatch()
{
    assert(!intr_enabled());
    if (Thread::ready_.empty()) {
        std::exit(0);
    }
    Thread *new_thread = Thread::ready_.front();
    Thread::ready_.pop();
    if (Thread::current() == nullptr) {
        Thread::cur_thread = new_thread;
        stack_switch(nullptr, new_thread->cur_stack);
    } else {
        Thread *temp = Thread::current();
        Thread::cur_thread = new_thread;
        stack_switch(temp->cur_stack, new_thread->cur_stack);
    }
}

void
Thread::yield()
{
    Thread::current()->schedule(); // check cur_thread
    Thread::redispatch();
}

void
Thread::exit()
{
    // You have to implement this
    if (last_deleted != nullptr) {
        delete last_deleted->cur_stack;
    }
    last_deleted = Thread::current();
    Thread::cur_thread = nullptr;
    Thread::redispatch();
    

    // Your code above should clean up the thread such that control never
    // reaches here (this method should never return).
    
    cerr << "Thread::exit() tried to return" << endl;
    abort();
}

void
Thread::preempt_init(uint64_t usec)
{
    // You have to implement this
}
