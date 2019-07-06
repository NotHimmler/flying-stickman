#ifndef EVENT_H
#define EVENT_H

#include <string>

using namespace std;

struct Event {
    string producer;
    string event;
    string name;
    int index;
};

#endif // EVENT_H
