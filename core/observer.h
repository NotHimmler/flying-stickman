#ifndef OBSERVER_H
#define OBSERVER_H

using namespace std;
class Subject;
#include <memory>
#include "event.h"

// Defines the interface for an observer of a subject
class Observer {
public:
    virtual void update(shared_ptr<const Event> evt) = 0;
};

#endif // OBSERVER_H
