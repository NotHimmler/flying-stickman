#include "jumpingstickmansubject.h"
#include "collision.h"
#include <iostream>

JumpingStickmanSubject::JumpingStickmanSubject(int floor, int jumpImpulse, int maxJumpCount, int gravity) : JumpingStickman(floor, jumpImpulse, maxJumpCount, gravity),
    observers()
{

}

void JumpingStickmanSubject::jump() {
    JumpingStickman::jump();
}

bool JumpingStickmanSubject::canJump() {
    return JumpingStickman::canJump();
}

void JumpingStickmanSubject::handleInput(QKeyEvent &event) {
    JumpingStickman::handleInput(event);
}

//Checks if a collision has occured and notifies observers
void JumpingStickmanSubject::update(std::vector<std::shared_ptr<Entity>> &obstacles) {
    Coordinate &ac = getCoordinate();
    int newY = ac.getYCoordinate() + jumpVelocity;
    colliding = false;
    // Check for collisions
    int index = 0;
    for (auto &other : obstacles) {
        Collision::CollisonResult col = Collision::moveCast(*this, *other, 0, jumpVelocity);

        if (col.overlapped) {
            int by = other->getCoordinate().getYCoordinate();
            if(other->getName().compare("coin") != 0 &&
                    other->getName().compare("heart") != 0 &&
                    other->getName().compare("star") != 0 &&
                    other->getName().compare("smaller") != 0 &&
                    other->getName().compare("bigger") != 0 &&
                    other->getName().compare("normal") != 0){
                if (col.down && jumpVelocity < 0) {
                    // Hitting obstacle from above
                    jumpVelocity = 0;
                    grounded = true;
                    jumpCount = 0;
                    newY = by + other->height() + 1;
                } else if (col.up) {
                    // Hitting obstacle from below
                    jumpVelocity = 0;
                    newY = by - height() - 1;
                } else {
                    colliding = true;
                    evt = make_shared<Event>();
                    evt->producer = "jumpingStickmanSubject";
                    evt->event = "collided";
                    evt->name = other->getName();
                    evt->index = index;
                }
            } else {
                // Hidding obstacle from the side
                colliding = true;
                evt = make_shared<Event>();
                evt->producer = "jumpingStickmanSubject";
                evt->event = "bonus";
                evt->index = index;
                evt->name = other->getName();

                if(other->getName().compare("smaller") == 0) {
                    this->setSize("tiny");
                } else if(other->getName().compare("bigger") == 0) {
                    this->setSize("large");
                } else if(other->getName().compare("star") == 0) {
                    this->setSize("huge");
                } else if(other->getName().compare("normal") == 0) {
                    this->setSize("normal");
                }
            }
        }
        index++;
    }


    if(this->size.compare("large") == 0){
        jumpImpulse = 20;
    } else {
        jumpImpulse = 15;
    }

    // Check if we're below the floor
    if (newY <= floor) {
        newY = floor;
        grounded = true;
        jumpVelocity = 0;
        jumpCount = 0;
    }

    ac.setYCoordinate(newY);
    jumpVelocity += gravity;
    if(colliding) {
        notify();
    }
}

void JumpingStickmanSubject::addObserver(shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void JumpingStickmanSubject::notify() {
    for(auto observer : observers) {
        observer->update(evt);
    }
}
