#ifndef STAGE3DECORATOR_H
#define STAGE3DECORATOR_H

#include <memory>
#include "gamestage.h"
#include "observer.h"
#include "event.h"

class Stage3Decorator : public GameStage, public Observer {
public:
    Stage3Decorator(std::shared_ptr<GameStage> stage, int lives = 3);
    ~Stage3Decorator();

    void update();
    void render(Renderer &renderer);
    void renderLives(Renderer &renderer);
    void renderLevel(Renderer &renderer);
    void input(QKeyEvent &event);
    void inputRelease(QKeyEvent &event);
    void setStage(std::shared_ptr<GameStage> stage);

    void updateIsColliding();
    void update(std::shared_ptr<const Event> evt);
    void updateLives(int lives);
    void updateLevel(int level);


private:
    std::shared_ptr<GameStage> stage;
    int lives;
    int level = 1;
};

#endif // STAGE3DECORATOR_H
