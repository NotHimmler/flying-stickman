#ifndef STAGE3DIALOG_H
#define STAGE3DIALOG_H

#include "dialog.h"
#include "stage3decorator.h"
#include "spriteflyweightfactory.h"
#include "observer.h"
#include "leaderboard.h"
#include <QObject>

class Stage3Dialog : public Dialog, public Observer {
public:
    /**
     * @brief Stage3Dialog
     * @param game The container of the dialog
     * @param stickman The player's stickman object
     * @param factory An entity factory for producing obstacles
     * @param obstacleLayout Pairs of (obstacle, space_until_next_obstacle) describing the sequence in which obstacles will be spawned
     */
    Stage3Dialog(Game &game, std::shared_ptr<Stickman> stickman, std::shared_ptr<EntityFactory> factory, std::vector<std::vector<std::pair<std::shared_ptr<Entity>, int>>> levelLayouts, int lives);
    virtual ~Stage3Dialog();
    void render(Renderer &renderer);
    void update();
    void renderObstacles(Renderer &renderer, unsigned int counter);
    virtual void input(QKeyEvent &event);
    virtual void inputRelease(QKeyEvent &event) override;
    void updateIsGrounded(bool isGrounded);
    void updateIsColliding();
    void update(std::shared_ptr<const Event> evt);
    int level;
    void addObserver(std::shared_ptr<Stage3Decorator> observer);

public slots:
    virtual void updateSeconds();

protected:
    virtual void spawnObstacles(unsigned int counter);
    virtual void updateObjectVelocities(int newVel);
    virtual void updateScoreEndOfLevel();
    virtual void incrementScoreByAmount(int amount);
    virtual void renderEndOfGameMessage(Renderer &renderer);
    virtual void restartGame();
    virtual void setGameStateAfterCollision();
    virtual void handleDirectionKeyPressState();
    virtual void setupNextLevel();
    virtual void setStartOfLevelDefaults();
    bool isObstacle(shared_ptr<Entity> entity);
    bool isOverlapping(std::shared_ptr<Entity> c);
    void checkAllObstaclesCollision();
    void potentiallySpawnPowerup();
    void potentiallySpawnCoin();
    void addEntityIfPossible(string name);

private:
    std::vector<std::vector<std::pair<std::shared_ptr<Entity>, int>>> levelLayouts;
    std::vector<std::pair<std::shared_ptr<Entity>, int>> obstacleLayout;
    SpriteFlyweightFactory flyweightFactory;
    int distanceToSpawn;
    int nextObstacle;
    bool stickmanGrounded = true;
    bool runningRightKeyPressed = false;
    bool runningLeftKeyPressed = false;
    bool collided = false;
    std::shared_ptr<Stage3Decorator> decoratorObserver;
    int elapsedSeconds = 0;
    int startOfLevelScore = 0;
    bool gameOver = false;
    int lives = 3;
    int defaultLives = 3;
    int stepsTaken = 0;
    int velocity = 0;
    string size = "normal";
    int timeSinceLastPowerup = 0;
    int obstaclesOffScreen = 0;
    int obstaclesAdded = 0;
    LeaderBoard leaderboard;
};

#endif // STAGE3DIALOG_H
