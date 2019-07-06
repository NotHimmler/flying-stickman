#include "entityfactory.h"

EntityFactory::EntityFactory() {

}

EntityFactory::~EntityFactory() {

}

void EntityFactory::setVelocity(int velocity) {
    this->velocity = velocity;
}

int EntityFactory::getVelocity() {
    return velocity;
}

std::shared_ptr<Entity> EntityFactory::getEntity(std::string name) {
    if (name == "bird") {
        Coordinate coordinate(800, 160, 450);
        auto randomBird = std::make_shared<Bird>(coordinate, velocity);
        randomBird->randomiseHeight();
        return std::move(randomBird);
    } else if (name == "cactus") {
        Coordinate coordinate(800, 150, 450);
        auto randomCactus = std::make_shared<Cactus>(coordinate, velocity);
        randomCactus->randomiseSprite();
        return std::move(randomCactus);
    } else if (name == "cloud") {
        Coordinate coordinate(800, 210, 450);
        auto randomCloud = std::make_shared<Cloud>(coordinate, velocity / 2);
        randomCloud->randomiseHeight();
        return std::move(randomCloud);
    } else if (name == "coin") {
        auto coin = std::make_shared<Coin>(spriteFactory.getFlyweight(name));
        return coin;
    } else if (name == "heart") {
        auto heart = std::make_shared<Heart>(spriteFactory.getFlyweight(name));
        return heart;
    } else if (name == "normal") {
        auto normal = std::make_shared<Normal>(spriteFactory.getFlyweight(name));
        return normal;
    } else if (name == "smaller") {
        auto smaller = std::make_shared<Smaller>(spriteFactory.getFlyweight(name));
        return smaller;
    } else if (name == "bigger") {
        auto bigger = std::make_shared<Bigger>(spriteFactory.getFlyweight(name));
        return bigger;
    } else if (name == "star") {
        auto star = std::make_shared<Star>(spriteFactory.getFlyweight(name));
        return star;
    }
}
