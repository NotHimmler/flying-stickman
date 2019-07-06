#include "leaderboard.h"
#include <QDate>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;

//For reverse sorting of highscores
bool leaderBoardSort (LeaderBoardScore a, LeaderBoardScore b) {
    return (a.score > b.score);
}

// Reads in high scores from the high scores file
// and sets up data structure
LeaderBoard::LeaderBoard() : scores()
{
    if(QFile::exists(QString::fromStdString("../Assignment3/highscores"))) {
        QFile scoresFile(QString::fromStdString("../Assignment3/highscores"));
        if (!scoresFile.open(QIODevice::ReadOnly | QIODevice::Text))
                return;
        QTextStream fileIn(&scoresFile);
        while(!fileIn.atEnd()) {
            QString line = fileIn.readLine();
            QStringList parts = line.split("|");
            if(parts.size() != 2) return;

            scores.push_back(LeaderBoardScore{parts[0].toStdString(), parts[1].toInt(), false});
        }

        scoresFile.close();
        sort(scores.begin(), scores.end(), leaderBoardSort);
    }
}

LeaderBoard::~LeaderBoard(){}

// Adds a score to the leaderboard and updates highscores file
void LeaderBoard::addScore(int score) {
    QDate today = QDate::currentDate();
    QString dateString = today.toString("dd.MM.yy");
    scores.push_back(LeaderBoardScore{dateString.toStdString(), score, true});
    sort(scores.begin(), scores.end(), leaderBoardSort);
    if (scores.size() > 5) {
        scores.pop_back();
    }

    //Overwrite old file if it exists
    if(QFile::exists(QString::fromStdString("../Assignment3/highscores"))) {
        QFile::remove(QString::fromStdString("../Assignment3/highscores"));
    }

    QFile scoresFile(QString::fromStdString("../Assignment3/highscores"));
    if (!scoresFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    QTextStream fileOut(&scoresFile);
    for(auto score : scores) {
        fileOut << QString::fromStdString(score.dateString) << "|" << score.score << "\n";
    }
    scoresFile.close();
}

// Renders the leaderboard with date and high scores
// If the last game was a high score, highlights in red
void LeaderBoard::renderLeaderboard(Renderer &renderer) {
    QPainter &painter = renderer.getPainter();
    QPen pen = painter.pen();
    QBrush brush = painter.brush();
    pen.setColor(Qt::darkGray);
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(250,160, 300, 240);
    painter.drawText(250,160,150,40,Qt::AlignHCenter, "Date");
    painter.drawText(400,160,150,40,Qt::AlignHCenter, "Score");

    int i = 1;
    for(auto score : scores) {

        if(score.isToday) {
            pen.setColor(Qt::darkRed);
            painter.setPen(pen);
        }

        painter.drawText(250,160 + 40*i,150,40,Qt::AlignHCenter, QString::fromStdString(score.dateString));
        painter.drawText(400,160 + 40*i,150,40,Qt::AlignHCenter, QString::fromStdString(std::to_string(score.score)));
        pen.setColor(Qt::darkGray);
        painter.setPen(pen);
        i++;
    }
}
