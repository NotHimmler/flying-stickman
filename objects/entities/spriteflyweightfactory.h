#ifndef SPRITEFLYWEIGHTFACTORY_H
#define SPRITEFLYWEIGHTFACTORY_H

#include <string>
#include <memory>
#include <QPixmap>

using namespace std;

class SpriteFlyweightFactory
{
public:
    SpriteFlyweightFactory();
    virtual ~SpriteFlyweightFactory();
    virtual shared_ptr<QPixmap> getFlyweight(string key);

private:
    map<string, shared_ptr<QPixmap>> flyweights;
};

#endif // FLYWEIGHTFACTORY_H
