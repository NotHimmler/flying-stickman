#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "renderer.h"
#include <string>

using namespace std;

struct LeaderBoardScore
{
    string dateString;
    int score;
    bool isToday;
};

class LeaderBoard
{
public:
    LeaderBoard();
    virtual ~LeaderBoard();
    void renderLeaderboard(Renderer &renderer);
    void addScore(int score);

private:
    vector<LeaderBoardScore> scores;
};

#endif // LEADERBOARD_H
