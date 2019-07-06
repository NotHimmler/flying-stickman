#include "stage3decorator.h"
#include "stage3dialog.h"
#include <string>
#include <memory>
#include <iostream>

Stage3Decorator::Stage3Decorator(std::shared_ptr<GameStage> stage, int lives) : stage(stage), lives(lives) {

}

Stage3Decorator::~Stage3Decorator() {

}

void Stage3Decorator::update() {
    stage->update();

}

void Stage3Decorator::render(Renderer &renderer) {
    renderLives(renderer);
    renderLevel(renderer);
    stage->render(renderer);
}

void Stage3Decorator::input(QKeyEvent &event) {
    stage->input(event);
}

void Stage3Decorator::inputRelease(QKeyEvent &event) {
    stage->inputRelease(event);
}

void Stage3Decorator::updateIsColliding() {
    lives--;
}

void Stage3Decorator::updateLives(int lives) {
    this->lives = lives;
}

void Stage3Decorator::updateLevel(int level) {
    this->level = level;
}

void Stage3Decorator::update(shared_ptr<const Event> evt) {
    Event event = *evt;
    if(event.producer.compare("jumpingStickmanSubject") == 0) {
        if(event.event.compare("collided") == 0 && event.name.compare("coin") == 0 && event.name.compare("heart") == 0) {
            //updateIsColliding();
        }
    }
}

//Draws the number of lives to the screen
void Stage3Decorator::renderLives(Renderer &renderer) {
    QPainter &painter = renderer.getPainter();
    QPen pen = painter.pen();
    pen.setColor(Qt::darkGray);
    painter.setPen(pen);
    std::string livesString = "Lives: " + std::to_string(lives);
    QString livesQString = QString::fromStdString(livesString);
    painter.drawText(50, 50, livesQString);
}

//Draws the current level to the screen
void Stage3Decorator::renderLevel(Renderer &renderer) {
    QPainter &painter = renderer.getPainter();
    QPen pen = painter.pen();
    pen.setColor(Qt::darkGray);
    painter.setPen(pen);
    std::string levelString = "Level: " + std::to_string(level);
    QString levelQString = QString::fromStdString(levelString);
    painter.drawText(50, 60, levelQString);
}
