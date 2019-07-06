#ifndef SUBJECT_H
#define SUBJECT_H
using namespace std;
#include "observer.h"
#include <memory>

// Defines the interface for the subject that will be observed
class Subject {
public:
    virtual void addObserver(shared_ptr<Observer> observer) = 0;
    virtual void notify() = 0;
};

#endif // SUBJECT_H
