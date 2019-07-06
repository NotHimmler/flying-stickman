#ifndef STICKMAN_H
#define STICKMAN_H

#include "gameobject.h"
#include "coordinate.h"
#include "renderer.h"
#include <string>
#include <QPainter>
#include <QKeyEvent>
#include <vector>
#include <memory>

class Entity;
class Stickman : public GameObject {
public:
    Stickman() = default;
    virtual ~Stickman();

    void setCoordinate(Coordinate coordinate);

    void setSize(std::string size);

    void setSprite(std::string path);

    Coordinate &getCoordinate();

    void render(Renderer &renderer, unsigned int time);

    virtual void handleInput(QKeyEvent &event);
    virtual void update(std::vector<std::shared_ptr<Entity>> &obstacles);

    bool isColliding();
    int width();
    int height();

protected:
    bool colliding = false;
    bool stoppedColliding = true;
    std::string size;

private:
    Coordinate coordinate;
    QPixmap sprite;
};

#endif // STICKMAN_H
