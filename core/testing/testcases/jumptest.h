#ifndef JUMPTEST_H
#define JUMPTEST_H

#include <memory>
#include "jumpingstickman.h"
#include "testrunner.h"
#include "entity.h"

class JumpTest : public TestRunner {
public:
    JumpTest();
    ~JumpTest() = default;

    void update();
    void render(Renderer &renderer);

private:
    int counter;
    std::shared_ptr<JumpingStickman> stickman;
    std::vector<std::shared_ptr<Entity>> obstacles;
};

#endif // JUMPTEST_H
