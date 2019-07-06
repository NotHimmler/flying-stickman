#ifndef GAMESTAGEDECORATOR_H
#define GAMESTAGEDECORATOR_H

#include <memory>
#include "gamestage.h"

class GameStageDecorator : public GameStage {
public:
    GameStageDecorator(std::shared_ptr<GameStage> stage);
    virtual ~GameStageDecorator() = 0;

    virtual void update();
    virtual void render(Renderer &renderer);
    virtual void input(QKeyEvent &event);

private:
    std::shared_ptr<GameStage> stage;
};

#endif // GAMESTAGEDECORATOR_H
