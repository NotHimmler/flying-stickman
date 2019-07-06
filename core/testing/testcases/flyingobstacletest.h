#ifndef FLYINGOBSTACLETESTCASE_H
#define FLYINGOBSTACLETESTCASE_H

#include "testrunner.h"
#include "jumpingstickman.h"

class FlyingObstacleTest : public TestRunner {
public:
    FlyingObstacleTest();
    ~FlyingObstacleTest() = default;

    void update();
    void render(Renderer &renderer);

private:
    int counter;
    std::shared_ptr<JumpingStickman> stickman;
    std::vector<std::shared_ptr<Entity>> obstacles;
};

#endif // FLYINGOBSTACLETESTCASE_H
