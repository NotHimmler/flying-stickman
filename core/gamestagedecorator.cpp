#include "gamestagedecorator.h"

GameStageDecorator::GameStageDecorator(std::shared_ptr<GameStage> stage) : stage(std::move(stage)) {

}

GameStageDecorator::~GameStageDecorator() {

}

void GameStageDecorator::update() {
    stage->update();
}

void GameStageDecorator::render(Renderer &renderer) {
    stage->render(renderer);
}

void GameStageDecorator::input(QKeyEvent &event) {
    stage->input(event);
}
