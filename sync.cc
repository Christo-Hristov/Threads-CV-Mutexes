
#include <iostream>
#include "sync.hh"
#include "timer.hh"

using namespace std;

Mutex::Mutex()
{
    locked = 0; // set locked to zero as mutex is unlocked upon initialization
    cur_owner = nullptr; // mutex has no owner upon intialization
}

void
Mutex::lock()
{
    IntrGuard ig; // disable interrupts to prevent race conditions
    if (mine()) {
        cerr << "locking mutex already locked by this thread" << endl;
        abort();
    }
    // if mutex is unlocked
    if (!locked) {
        locked = 1; // lock mutex
        cur_owner = Thread::current(); // set owner of mutex to thread locking mutex
    // if mutex is locked
    } else {
        blocked.push(Thread::current()); // block current thread 
        Thread::redispatch(); // switch to next thread
    }
}

void
Mutex::unlock()
{
    IntrGuard ig; // disable interrupts to prevent race conditions
    if (!mine()) {
        cerr << "unlocking mutex not held by this thread" << endl;
        abort();
    }
    // if no threads are waiting on mutex
    if (blocked.empty()) {
        locked = 0; // set mutex to unlocked
        cur_owner = nullptr; // might need to change
    // if threads are waiting on mutex
    } else {
        cur_owner = blocked.front(); // set current owner to next thread that wil have mutex
        blocked.front()->schedule(); // unblock next thread in blocked list
        blocked.pop(); // remove thread from blocked list
    }
}

bool
Mutex::mine()
{
    return (Thread::current() == cur_owner);
}

Condition::Condition()
{

}

void
Condition::wait(Mutex &m)
{
    IntrGuard ig;
    if (!m.mine()) {
        cerr << "Condition::wait must be called with mutex locked"
                << endl;
        abort();
    }
    m.unlock();
    blocked.push(Thread::current());
    Thread::redispatch();
    m.lock();
}

void
Condition::notify_one()
{
    IntrGuard ig;
    if (!blocked.empty()){
        blocked.front()->schedule();
        blocked.pop();
    }
}

void
Condition::notify_all()
{
    IntrGuard ig;
    while (!blocked.empty()) {
        blocked.front()->schedule();
        blocked.pop();
    }
    
}
