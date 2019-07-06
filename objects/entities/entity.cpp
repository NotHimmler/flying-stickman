#include <QBitmap>
#include "entity.h"
#include <iostream>

using namespace std;

Entity::Entity(std::string name, Coordinate coordinate, int velocity):
    name(name),
    coordinate(coordinate),
    velocity(velocity),
    widthOverride(-1),
    heightOverride(-1) {
    updateSprite(0);
}

std::shared_ptr<Entity> Entity::clone() {
    auto e = std::make_shared<Entity>(name, coordinate, velocity);
    e->setSize(widthOverride, heightOverride);
    return e;
}

Coordinate &Entity::getCoordinate() {
    return coordinate;
}

void Entity::setVelocity(int v) {
    velocity = v;
}

int Entity::getVelocity() {
    return velocity;
}

std::string Entity::getName() {
    return name;
}

int Entity::width() {
    return widthOverride >= 0 ? widthOverride : sprite.width();
}

int Entity::height() {
    return heightOverride >= 0 ? heightOverride : sprite.height();
}

void Entity::setSize(int width, int height) {
    widthOverride = width;
    heightOverride = height;
}

void Entity::collisionLogic(Stickman &player) {
    if (!player.isColliding()) this->updateCoordinate();
}

void Entity::updateCoordinate() {
    coordinate.setXCoordinate(coordinate.getQtRenderingXCoordinate() - velocity);
}

void Entity::updateSprite(unsigned int time) {
    if (name == "cactus" || name == "cloud") {
        return;
    }
    std::string spritePath = ":sprites/" + name + std::to_string((time/10)%2).append(".png");
    QPixmap sprite(QString::fromStdString(spritePath));
    setSprite(sprite);
}

QPixmap &Entity::getSprite() {
    return sprite;
}

void Entity::setSprite(QPixmap &pm) {
    if (widthOverride >= 0 && heightOverride >= 0) {
        this->sprite = pm.scaled(widthOverride, heightOverride);
    } else {
        this->sprite = pm;
    }
}

void Entity::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - sprite.height(), sprite);
    updateSprite(time);
}

Bird::Bird(Coordinate coordinate, int velocity):
    Entity("bird", coordinate, velocity) {}

// Spawn bird at random height in the sky
void Bird::randomiseHeight() {
    this->coordinate.setYCoordinate(150 + 35*(rand() % 4));
}

Cactus::Cactus(Coordinate coordinate, int velocity):
    Entity("cactus", coordinate, velocity) {}

// Choose from 3 random cactuses
void Cactus::randomiseSprite() {
    std::string spritePath = ":sprites/" + name + std::to_string(rand() % 3) + ".png";
    QPixmap sprite(QString::fromStdString(spritePath));
    setSprite(sprite);
}

Cloud::Cloud(Coordinate coordinate, int velocity):
    Entity("cloud", coordinate, velocity) {
    std::string spritePath = ":/sprites" + name + "0.png";
    QPixmap sprite(QString::fromStdString(spritePath));
    this->sprite = sprite;
}

// Clouds can spawn at random height in the sky
void Cloud::randomiseHeight() {
    this->coordinate.setYCoordinate(coordinate.getQtRenderingYCoordinate() + 30*(rand() % 6));
}

Coin::Coin(std::shared_ptr<QPixmap> sprite) : Entity() {
     coordinate = Coordinate(800, 160, 450);
     this->velocity = velocity;
     this->coinSprite = sprite;
     this->name = "coin";
}

void Coin::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*coinSprite).height(), *coinSprite);
}

void Coin::setCollided() {
    collided = true;
}

bool Coin::getCollided() {
    return collided;
}

bool Coin::checkCoinCollision(shared_ptr<Stickman> sm){
    return checkCollision(sm, this);
}

bool checkCollision(shared_ptr<Stickman> sm, Entity * ePtr) {
    int smx = sm->getCoordinate().getQtRenderingXCoordinate();
    int smy = sm->getCoordinate().getQtRenderingYCoordinate();
    int cx = ePtr->getCoordinate().getQtRenderingXCoordinate();
    int cy = ePtr->getCoordinate().getQtRenderingYCoordinate();
    int yDiff = (smy - cy);
    int xDiff = (smx - cx);
    yDiff = (yDiff < 0) ? yDiff*-1 : yDiff;
    xDiff = (xDiff < 0) ? xDiff*-1 : xDiff;
    if(xDiff <= 20 && yDiff <= 20) {
        return true;
    }
    return false;
}


Heart::Heart(shared_ptr<QPixmap> heartSprite) : Entity() {
     coordinate = Coordinate(800, 220, 450);
     this->velocity = velocity;
     this->heartSprite = heartSprite;
     this->name = "heart";
}

void Heart::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*heartSprite).height(), *heartSprite);
}

void Heart::setCollided() {
    collided = true;
}

bool Heart::getCollided() {
    return collided;
}

bool Heart::checkHeartCollision(shared_ptr<Stickman> sm){
    return checkCollision(sm, this);
}

Star::Star(shared_ptr<QPixmap> starSprite) : Entity() {
     coordinate = Coordinate(800, 220, 450);
     this->velocity = velocity;
     this->starSprite = starSprite;
     this->name = "star";
}

void Star::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*starSprite).height(), *starSprite);
}

Bigger::Bigger(shared_ptr<QPixmap> biggerSprite) : Entity() {
     coordinate = Coordinate(800, 220, 450);
     this->velocity = velocity;
     this->biggerSprite = biggerSprite;
     this->name = "bigger";
}

void Bigger::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*biggerSprite).height(), *biggerSprite);
}

Smaller::Smaller(shared_ptr<QPixmap> smallerSprite) : Entity() {
     coordinate = Coordinate(800, 220, 450);
     this->velocity = velocity;
     this->smallerSprite = smallerSprite;
     this->name = "smaller";
}

void Smaller::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*smallerSprite).height(), *smallerSprite);
}

Normal::Normal(shared_ptr<QPixmap> normalSprite) : Entity() {
     coordinate = Coordinate(800, 220, 450);
     this->velocity = velocity;
     this->normalSprite = normalSprite;
     this->name = "normal";
}

void Normal::render(Renderer &renderer, unsigned int time) {
    renderer.draw(coordinate.getQtRenderingXCoordinate(), coordinate.getQtRenderingYCoordinate() - (*normalSprite).height(), *normalSprite);
}
