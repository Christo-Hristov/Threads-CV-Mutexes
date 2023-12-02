
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

Thread* Thread::cur_thread = nullptr; // intialize to nullptr as there is no running thread upon calling the class
Stack* Thread::last_deleted = nullptr; // initialize to nullptr as no thread has been deleted upon initialization of class

void
Thread::thread_start(Thread *t)
{
    intr_enable(true); // manually enable interrupts as first time in thread we don't exit from timer handler
    t->my_func(); // call function of thread
    Thread::exit(); // exit thread once function has returned
}

Thread::Thread(function<void()> main) : my_func(main)
{
    cur_stack = new Stack(Thread::thread_start, this); // create a stack for the thread
    Thread::ready_.push(this); // add thread to ready queue
}

Thread *
Thread::current()
{
    return Thread::cur_thread; // returns the current thread
}

void
Thread::schedule()
{
    IntrGuard ig; // enables interrupt so we are not interrupted while changign global queue
    Thread::ready_.push(this); // add thread to back of ready queue
    
}

void
Thread::redispatch()
{
    IntrGuard ig; // enable interrupts
    assert(!intr_enabled());
    // if there are no more ready threads, terminate program
    if (Thread::ready_.empty()) {
        std::exit(0);
    }
    Thread *new_thread = Thread::ready_.front(); // store next ready thread
    Thread::ready_.pop(); // remove next ready thread from ready queue
    // if no thread is currently running
    if (Thread::current() == nullptr) {
        Thread::cur_thread = new_thread; // set cur thread to thread we are switching to 
        stack_switch(nullptr, new_thread->cur_stack); // switch to next thread
    } else {
        Thread *temp = Thread::current();
        Thread::cur_thread = new_thread; // set cur_thread to thread we are switching to 
        stack_switch(temp->cur_stack, new_thread->cur_stack);
    }
}

void
Thread::yield()
{
    IntrGuard ig; // disable interrupts to prevent interrupt occuring after just scheduling
    Thread::current()->schedule(); 
    Thread::redispatch();
}

void
Thread::exit()
{
    IntrGuard ig; // enable interrupts because changing global variables
    if (last_deleted != nullptr) { // if a thread has been deleted
        delete last_deleted; // free the memory for the stack of the previously deleted thread
    }
    
    last_deleted = Thread::current()->cur_stack; // set previously deleted thread to current thread
    delete Thread::current(); // delete memory for all parameters of current thread except its stack
    Thread::cur_thread = nullptr; // indicate there is no thread running
    Thread::redispatch(); // switch to next thread
    

    // Your code above should clean up the thread such that control never
    // reaches here (this method should never return).
    
    cerr << "Thread::exit() tried to return" << endl;
    abort();
}

void
Thread::handler()
{
    Thread::yield(); // add current thread to back of ready queue and switch to new thread
}

void
Thread::preempt_init(uint64_t usec)
{
    timer_init(usec, Thread::handler); // initialize timer with timer handler
}
