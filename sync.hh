
#pragma once


#include "thread.hh"

// A standard mutex providing mutual exclusion.
class Mutex {
public:
    // This is the constructor for `Mutex` objects. It has no arguments.
    Mutex();

    // Locks the Mutex, blocking (if necessary) until the Mutex is unlocked.
    // Blocked threads return in FIFO order (first to block is first to
    // acquire the lock).
    void lock();

    // If no threads are waiting for the Mutex, releases the lock; otherwise,
    // passes ownership of the Mutex to the first waiting thread and wakes
    // it up.
    void unlock();

    // Returns true if the lock is held by the current thread
    bool mine();

    // These lines prevent Mutexes from being copied.
    Mutex(const Mutex &) = delete;
    Mutex &operator=(const Mutex &) = delete;

private:
    // You must define appropriate variables for this object
};

// A condition variable; methods are similar to std::condition_variable_any
// except that wait takes a Mutex as an argument rather than a
// std::mutex.
class Condition {
public:
    // This is the constructor for `Condition` objects.  It takes no arguments.
    Condition();

    // Releases the Mutex and blocks the thread until notify_one or
    // notify_all has been invoked, then reacquires the Mutex. The
    // caller must have locked m before calling this method.
    void wait(Mutex &m);

    // If any threads are blocked on the condition variable, wakes up the
    // one that has been blocked the longest. If no threads are blocked,
    // then this method does nothing.
    void notify_one();

    // Wakes up all of the threads currently blocked on the condition variable.
    void notify_all();

private:
    // You must define appropriate variables for this object
};
