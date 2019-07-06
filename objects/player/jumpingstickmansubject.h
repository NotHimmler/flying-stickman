#ifndef JUMPINGSTICKMANSUBJECT_H
#define JUMPINGSTICKMANSUBJECT_H

#include "jumpingstickman.h"
#include "stage3decorator.h"
#include "stage3dialog.h"
#include "subject.h"
#include <memory>

class JumpingStickmanSubject: public JumpingStickman, public Subject
{
public:
    JumpingStickmanSubject(int floor, int jumpImpulse = 15, int maxJumpCount = 2, int gravity = -1);

    void jump();
    bool canJump();

    void handleInput(QKeyEvent &event);
    void update(std::vector<std::shared_ptr<Entity>> &obstacles);

    virtual void addObserver(shared_ptr<Observer> observer);
    virtual void notify();

private:
    std::vector<std::shared_ptr<Observer>> observers;
    string eventStr = "";
    shared_ptr<Event> evt;
    bool notifiedGrounded = false;
    bool notifiedCollided = false;
};

#endif // JUMPINGSTICKMANSUBJECT_H
