
#pragma once

#include <queue>
#include <functional>

#include "stack.hh"

class Thread {
public:
    // Constructs a new thread that will run the specified function (which
    // takes no arguments and returns no result. Adds the new thread to the
    // back of the ready queue.  If main returns, the thread is terminated
    // just as if it had invoked Thread::exit.
    Thread(std::function<void()> main);

    // Adds the thread to the back of the ready queue so that it will
    // eventually run when the CPU is available. (The thread should be
    // ready when you call this function.)
    void schedule();

    // Allows other threads to execute, if there are any (calls schedule
    // on the current thread and then redispatches to the next ready thread).
    static void yield();

    // Returns a pointer to the currently running thread or nullptr if 
    // no thread is currently running.
    static Thread *current();

    // Stops executing the current thread and switches to the next ready
    // thread. The caller must make sure that the current thread has
    // been scheduled or kept track of in some other way before calling
    // this; otherwise it will never run again. Must be invoked with
    // interrupts disabled.  If there are no more ready threads, this
    // method terminates the entire program by calling std::exit(0).
    static void redispatch();

    // Terminates the current thread and then runs other ready threads,
    // if any. This method never returns, since the thread ceases to exist.
    // (thus we will switch to another thread and never switch back)
    [[noreturn]] static void exit();

    // Initializes preemptive threading.  If this function is called
    // once at the start of the program, the currently running thread
    // will be preempted every usec microseconds.
    // If this method is not called, then your dispatcher should 
    // be non-preemptive (threads run until they either yield or exit).
    static void preempt_init(std::uint64_t usec = 100'000);

private:
    // A list of threads that are ready to execute. The "static" keyword
    // means that there is only a single instance of this variable, which
    // is shared across all Thread instances.
    static std::queue<Thread*> ready_;

    // It's important that the destructor is private: threads can only
    // be deleted when they invoke exit.
    ~Thread() {};

    // The top-level function that is invoked when a thread first starts
    // running. This function peforms any necessary intialization, invokes
    // the main function for the thread, then invokes Thread::exit if that
    // function returns.
    // The thread being run is passed in as the first parameter.
    [[noreturn]] static void thread_start(Thread *t);

    // Fill in other fields and/or methods that you need.
    static Stack *last_deleted;
    static Thread *cur_thread; // pointer to the current running thread
    Stack *cur_stack; // pointer to the stack which contains thread info
    std::function<void()> my_func; // function the thread will execute
};
