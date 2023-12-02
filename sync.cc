
#include <iostream>
#include "sync.hh"
#include "timer.hh"

using namespace std;

Mutex::Mutex()
{
    locked = 0;
    cur_owner = nullptr;
}

void
Mutex::lock()
{
    IntrGuard ig;
    if (mine()) {
        cerr << "locking mutex already locked by this thread" << endl;
        abort();
    }
    
    if (!locked) {
        locked = 1;
        cur_owner = Thread::current();
    } else {
        blocked.push(Thread::current());
        Thread::redispatch();
    }
}

void
Mutex::unlock()
{
    if (!mine()) {
        cerr << "unlocking mutex not held by this thread" << endl;
        abort();
    }

    if (blocked.empty()) {
        locked = 0;
    } else {
        cur_owner = blocked.front();
        blocked.front()->schedule();
        blocked.pop();
    }
}

bool
Mutex::mine()
{
    // Replace the code below with your implementation.
    return true;
}

Condition::Condition()
{
    // Fill in any code that is needed here.
}

void
Condition::wait(Mutex &m)
{
    if (!m.mine()) {
        cerr << "Condition::wait must be called with mutex locked"
                << endl;
        abort();
    }

    // You need to implement the rest of this function
}

void
Condition::notify_one()
{
    // You need to implement this function
}

void
Condition::notify_all()
{
    // You need to implement this function
}
