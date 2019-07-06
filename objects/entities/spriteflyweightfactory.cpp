#include "spriteflyweightfactory.h"
#include <iostream>

SpriteFlyweightFactory::SpriteFlyweightFactory() : flyweights()
{

}

SpriteFlyweightFactory::~SpriteFlyweightFactory()
{

}

shared_ptr<QPixmap> SpriteFlyweightFactory::getFlyweight(string key) {
    //See if the pixmap exists
    auto iterator = flyweights.find(key);

    //Pixmap not yet in map
    if(iterator == flyweights.end()) {
         string pathName = ":sprites/" + key + ".png";
         auto pixmapPtr = make_shared<QPixmap>(QString::fromStdString(pathName));
         auto pair = std::pair<string, shared_ptr<QPixmap>>(key, pixmapPtr);
         flyweights.insert(pair);
         return pixmapPtr;
    } else {
        auto pair = *iterator;
        shared_ptr<QPixmap> ptr = pair.second;
        return ptr;
    }
}
