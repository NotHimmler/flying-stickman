#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <cstdlib>
#include <string>
#include <QPixmap>
#include <QPainter>
#include <memory>
#include "stickman.h"
#include "coordinate.h"
#include "gameobject.h"
#include "renderer.h"

class Entity : public GameObject {
public:
    Entity() = default;
    Entity(std::string name, Coordinate coordinate, int velocity);
    virtual ~Entity() = default;

    virtual void collisionLogic(Stickman &player);
    virtual void updateCoordinate();
    virtual void render(Renderer &renderer, unsigned int time);

    virtual Coordinate &getCoordinate();
    virtual QPixmap &getSprite();
    virtual int width();
    virtual int height();
    virtual void setSize(int width, int height);
    virtual int getVelocity();
    virtual void setVelocity(int v);
    virtual std::string getName();

    virtual std::shared_ptr<Entity> clone();

protected:
    std::string name;
    Coordinate coordinate;
    int velocity;
    QPixmap sprite;
    int widthOverride;
    int heightOverride;

    void setSprite(QPixmap &pm);
    void updateSprite(unsigned int time);
};

class Bird: public Entity {
public:
    Bird(Coordinate coordinate, int velocity);

    void randomiseHeight();
};

class Cactus: public Entity {
public:
    Cactus(Coordinate coordinate, int velocity);

    void randomiseSprite();
};

class Cloud: public Entity {
public:
    Cloud(Coordinate coordinate, int velocity);

    void randomiseHeight();
};

class Coin: public Entity {
public:
    Coin(std::shared_ptr<QPixmap> sprite);
    void setCollided();
    bool getCollided();
    void render(Renderer &renderer, unsigned int time);
    bool checkCoinCollision(std::shared_ptr<Stickman> sm);

private:
    bool collided = false;
    std::shared_ptr<QPixmap> coinSprite;
};

class Heart: public Entity {
public:
    Heart(std::shared_ptr<QPixmap> coinSprite);
    void setCollided();
    bool getCollided();
    void render(Renderer &renderer, unsigned int time);
    bool checkHeartCollision(std::shared_ptr<Stickman> sm);

private:
    bool collided = false;
    std::shared_ptr<QPixmap> heartSprite;
};

class Star: public Entity {
public:
    Star(std::shared_ptr<QPixmap> starSprite);
    void render(Renderer &renderer, unsigned int time);

private:
    std::shared_ptr<QPixmap> starSprite;
};

class Smaller: public Entity {
public:
    Smaller(std::shared_ptr<QPixmap> smallerSprite);
    void render(Renderer &renderer, unsigned int time);

private:
    std::shared_ptr<QPixmap> smallerSprite;
};

class Bigger: public Entity {
public:
    Bigger(std::shared_ptr<QPixmap> biggerSprite);
    void render(Renderer &renderer, unsigned int time);

private:
    std::shared_ptr<QPixmap> biggerSprite;
};

class Normal: public Entity {
public:
    Normal(std::shared_ptr<QPixmap> normalSprite);
    void render(Renderer &renderer, unsigned int time);

private:
    std::shared_ptr<QPixmap> normalSprite;
};

bool checkCollision(std::shared_ptr<Stickman>, Entity *ePtr);

#endif // OBSTACLE_H
