
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

void
Thread::thread_start(Thread *t)
{
    t->my_func();
    while (1) {}
}

Thread::Thread(function<void()> main) : my_func(main)
{
    cur_stack = new Stack(Thread::thread_start, this);
}

Thread *
Thread::current()
{
    // Replace the code below with your implementation.
    return nullptr;
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
    Thread *new_thread = Thread::ready_.front();
    Thread::ready_.pop();
    stack_switch(nullptr, new_thread->cur_stack);
}

void
Thread::yield()
{
    // You have to implement this
}

void
Thread::exit()
{
    // You have to implement this

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
