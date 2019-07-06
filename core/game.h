#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <QKeyEvent>
#include <memory>
#include "gamestage.h"

class Game : public QDialog {
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game() = default;

    void setStage(std::shared_ptr<GameStage> stage);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    std::shared_ptr<GameStage> stage;

public slots:
    void nextFrame();
};

#endif // GAME_H
