#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <stack>
#include <QPainter>
#include <QPixmap>
#include "renderer.h"

class Score {
public:
    Score();

    void increment();
    void render(Renderer &renderer);
    void setScore(unsigned int scoreToSet);
    unsigned int getScore();

private:
    unsigned int hiscore;
    unsigned int currScore;
    QPixmap digits[10];
};

#endif // SCORE_H
