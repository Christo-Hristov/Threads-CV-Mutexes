
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
    // Replace the line below with a real implementation
    while (1) {}
}

Thread::Thread(function<void()> main)
{
    // You have to implement this
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
    // You have to implement this
}

void
Thread::redispatch()
{
    assert(!intr_enabled());
    // You have to implement this
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
