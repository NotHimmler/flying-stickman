#ifndef DIALOG_H
#define DIALOG_H

#include <vector>
#include <QDialog>
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <memory>
#include "stickman.h"
#include "background.h"
#include "entity.h"
#include "moon.h"
#include "score.h"
#include "entityfactory.h"
#include "game.h"
#include "renderer.h"

class Dialog : public GameStage {
public:
    Dialog(Game &game, std::shared_ptr<Stickman> stickman, std::shared_ptr<EntityFactory> factory);
    ~Dialog();

    virtual void update();
    virtual void render(Renderer &renderer);
    virtual void input(QKeyEvent &event);

    void setStickman(std::shared_ptr<Stickman> stickman);
    void setBackground(Background background);
    void setMoon(Moon moon);

protected:
    virtual void spawnObstacles(unsigned int counter);
    void addObstacle(std::shared_ptr<Entity> obstacle);
    void speedUp(unsigned int counter);

    void renderBackground(Renderer &renderer, unsigned int counter);
    void renderClouds(Renderer &renderer, unsigned int counter);
    void renderObstacles(Renderer &renderer, unsigned int counter);

    Game &game;
    std::shared_ptr<Stickman> stickman;
    std::shared_ptr<EntityFactory> factory;
    Background background;
    std::vector<std::shared_ptr<Entity>> obstacles;
    std::vector<std::shared_ptr<Entity>> clouds;
    Score score;
    int counter;

private:
    bool night;
    Moon moon;
    unsigned int obstacleSpawnFrame;
    unsigned int cloudSpawnFrame;
};

#endif // DIALOG_H
