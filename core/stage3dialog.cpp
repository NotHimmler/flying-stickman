#include "stage3dialog.h"
#include "collision.h"
#include "debugrenderer.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Stage3Dialog::Stage3Dialog(Game &game, std::shared_ptr<Stickman> stickman, std::shared_ptr<EntityFactory> factory, std::vector<std::vector<std::pair<std::shared_ptr<Entity>, int>>> levelLayouts, int lives) :
    Dialog(game, std::move(stickman), std::move(factory)), levelLayouts(levelLayouts), distanceToSpawn(0), nextObstacle(0), obstacleLayout(levelLayouts.front()), level(0), lives(lives), defaultLives(lives), flyweightFactory()
{
    score.setScore(0);
}

Stage3Dialog::~Stage3Dialog() {

}

void Stage3Dialog::spawnObstacles(unsigned int /*counter*/) {
    //Spawn a coin with random chance when an obstacle is not to be spawned
    if(distanceToSpawn > 0 && timeSinceLastPowerup > 20 && obstaclesAdded != obstaclesOffScreen) {
        potentiallySpawnCoin();
        potentiallySpawnPowerup();
    } else {
        timeSinceLastPowerup++;
    }

    // Check if it's time to spawn an obstacle
    if (obstacleLayout.size() == 0 || distanceToSpawn > 0 || nextObstacle >= obstacleLayout.size()) return;

    auto &e = obstacleLayout[nextObstacle];

    // Only spawn the obstacle if it isn't colliding with anything
    if (!isOverlapping(e.first)) {
        auto obs = e.first->clone();
        obs->setVelocity(background.getVelocity());
        addObstacle(std::move(obs));
        obstaclesAdded++;
        //makes sure that obstacles dont overlap with coins/powerups
        timeSinceLastPowerup = 0;
    }

    // Set next obstacle in sequence
    distanceToSpawn = e.second;

    nextObstacle++;
}

void Stage3Dialog::potentiallySpawnPowerup()
{
    int num = rand() % 2000;
    if(num < 33){
        addEntityIfPossible("heart");
    } else if(num >= 33 && num < 66) {
        addEntityIfPossible("star");
    } else if(num >= 66 && num < 99) {
        addEntityIfPossible("normal");
    } else if(num >= 99 && num < 132) {
        addEntityIfPossible("bigger");
    } else if(num >= 132 && num < 165) {
        addEntityIfPossible("smaller");
    }
}

// Adds an entity to the obstacles if it doesn't
// overlap another
void Stage3Dialog::addEntityIfPossible(string name)
{
    auto entity = factory->getEntity(name);
    entity->setVelocity(5 + 2*(level + 1));
    if (!isOverlapping(entity)) {
        entity->setVelocity(background.getVelocity());
        addObstacle(entity);
        timeSinceLastPowerup = 0;
    }
}

//Spawns a coin with random chance
void Stage3Dialog::potentiallySpawnCoin()
{
    int num = rand() % 100;
    if(num < 33) {
        auto c = factory->getEntity("coin");
        c->setVelocity(5 + 2*(level + 1));
        // Only spawn the obstacle if it isn't colliding with anything
        if (!isOverlapping(c)) {
            c->setVelocity(background.getVelocity());
            addObstacle(c);
            timeSinceLastPowerup = 0;
        }
    }
}

//Checks if two entities would be overlapping
bool Stage3Dialog::isOverlapping(shared_ptr<Entity> c)
{
    bool isOverlapping = false;
    for (auto &o : obstacles) {
        if (Collision::overlaps(*c, *o)) {
            isOverlapping = true;
            break;
        }
    }

    return isOverlapping;
}

void Stage3Dialog::update() {
    //Keeps track of elapsed seconds for score calculation
    if(counter % 67 == 0) {
        updateSeconds();
    }

    handleDirectionKeyPressState();

    if(collided) {
        setGameStateAfterCollision();
    }

    if(!gameOver) {
        checkAllObstaclesCollision();

        if (!stickman->isColliding()) {
            // Reduce distance to next obstacle
            distanceToSpawn -= background.getVelocity();
            background.update();
        }
        spawnObstacles(counter);
        //End of level has been reached
        if(obstaclesAdded == obstaclesOffScreen && (level + 1) < levelLayouts.size()) {
            setupNextLevel();
        //End of game has been reached
        } else if (obstaclesAdded != 0 && obstaclesAdded == obstaclesOffScreen && (level + 1) == levelLayouts.size()) {
            updateScoreEndOfLevel();
            updateObjectVelocities(0);
            leaderboard.addScore(int(score.getScore()));
            gameOver = true;
        }

    }
}

void Stage3Dialog::checkAllObstaclesCollision()
{
    stickman->update(obstacles);
    for (auto &c : clouds) c->collisionLogic(*stickman);

    bool obstaclesNowOffscreen = false;
    bool obstacleWasOffscreen = false;
    for (auto &o : obstacles) {
        //reset for each obstacle
        obstaclesNowOffscreen = false;
        obstacleWasOffscreen = false;

        //Check if obstacle was offscreen
        if (o->getCoordinate().getQtRenderingXCoordinate() + o->width() < 0 && isObstacle(o)) {
            obstacleWasOffscreen = true;
        }
        //This method moves the entity as well as checking for a collision....
        o->collisionLogic(*stickman);

        //Check if obstacle is now on screen
        if (o->getCoordinate().getQtRenderingXCoordinate() + o->width() < 0 && isObstacle(o)) {
          obstaclesNowOffscreen = true;
        }

        //Keep track of obstacles coming on and going off screen
        if (!obstacleWasOffscreen && obstaclesNowOffscreen) {
          obstaclesOffScreen++;
        } else if(obstacleWasOffscreen && !obstaclesNowOffscreen) {
          obstaclesOffScreen--;
        }
    }
}

//Resets the dialog's state after having lost a life
//Sets game over if lives are 0
void Stage3Dialog::setGameStateAfterCollision()
{
    stickman->setSize("normal");
    this->size = "normal";
    lives--;
    decoratorObserver->updateLives(lives);
    obstacles = vector<shared_ptr<Entity>>();
    nextObstacle = 0;
    distanceToSpawn = 0;
    obstaclesOffScreen = 0;
    obstaclesAdded = 0;
    collided = false;
    if(lives == 0) {
        updateScoreEndOfLevel();
        gameOver = true;
    }
    score.setScore(startOfLevelScore);
    leaderboard.addScore(score.getScore());
}

//Loads next level obstacles and sets appropriate state values
void Stage3Dialog::setupNextLevel()
{
    updateScoreEndOfLevel();
    startOfLevelScore = score.getScore();
    level++;
    setStartOfLevelDefaults();
}

//Reset state values for beginning the game from the first level
void Stage3Dialog::restartGame() {
    level = 0;
    setStartOfLevelDefaults();
    startOfLevelScore = 0;
    score.setScore(0);
    lives = defaultLives;
    decoratorObserver->updateLives(lives);
    gameOver = false;
}

// Sets the member values for the start of a level
void Stage3Dialog::setStartOfLevelDefaults() {
    this->stickman->setSize("normal");
    this->size = "normal";
    obstaclesAdded = 0;
    obstaclesOffScreen = 0;
    obstacleLayout = levelLayouts[level];
    obstacles = vector<shared_ptr<Entity>>();
    distanceToSpawn = 0;
    nextObstacle = 0;
    stepsTaken = 0;
    decoratorObserver->updateLevel(level + 1);
    updateObjectVelocities(5 + 2*(level+1));
    elapsedSeconds = 0;
}

//Handle Key Inputs
void Stage3Dialog::input(QKeyEvent &event) {
    if (event.key() == Qt::Key_Space) {
        stickman->handleInput(event);
    } else if (event.key() == Qt::Key_Right && !event.isAutoRepeat() && !gameOver) {
        runningRightKeyPressed = true;
    } else if (event.key() == Qt::Key_Left && !event.isAutoRepeat() && !gameOver) {
        runningLeftKeyPressed = true;
    } else if(gameOver && event.key() == Qt::Key_R) {
        restartGame();
    }
}

//Handle Key Inputs
void Stage3Dialog::inputRelease(QKeyEvent &event) {
    if (event.key() == Qt::Key_Right && !event.isAutoRepeat()) {
        runningRightKeyPressed = false;
    } else if (event.key() == Qt::Key_Left && !event.isAutoRepeat()) {
        runningLeftKeyPressed = false;
    }

}

//Sets object velocities depending on forwards or backwards key pressed
void Stage3Dialog::handleDirectionKeyPressState()
{
    if (!(runningRightKeyPressed != runningLeftKeyPressed) && velocity != 0 && stickmanGrounded) {
        updateObjectVelocities(0);
    }else if(runningRightKeyPressed) {
        if(velocity != (5+2*(level+1) || velocity == 0)) updateObjectVelocities(5 + 2*(level+1));
        stepsTaken++;
    } else if (runningLeftKeyPressed) {
        //Only allows to go back to the start of the level
        if(stepsTaken > 0) {
            if(velocity != -1*(5+2*(level+1) || velocity == 0)) updateObjectVelocities(-1*(5 + 2*(level+1)));
            stepsTaken--;
        } else if (velocity != 0) {
            updateObjectVelocities(0);
        }
    }
}

void Stage3Dialog::updateIsGrounded(bool isGrounded) {
    this->stickmanGrounded = isGrounded;
}

void Stage3Dialog::updateIsColliding() {
    collided = true;
}

void Stage3Dialog::addObserver(std::shared_ptr<Stage3Decorator> observer) {
    decoratorObserver = observer;
}

void Stage3Dialog::render(Renderer &renderer) {
    renderBackground(renderer, counter);
    renderObstacles(renderer, counter);
    stickman->render(renderer, counter);
    if(gameOver) renderEndOfGameMessage(renderer);
    score.render(renderer);
    counter++;
}

//Set the velocities of objects to a new velocity
//Update the state velocity variable
void Stage3Dialog::updateObjectVelocities(int newVel) {
    velocity = newVel;

    background.setVelocity(newVel);

    for(auto entity : obstacles) entity->setVelocity(newVel);
}

//Self explanatory
void Stage3Dialog::updateSeconds() {
    elapsedSeconds++;
}

//Adds a value to the previous and coin score for a level
//Based on the total distance and time and level
void Stage3Dialog::updateScoreEndOfLevel() {
    int totalSpace = 0;
    for(auto pair : levelLayouts[level]) {
        totalSpace += pair.second;
    }

    int scoreToAdd = max((totalSpace/10 - elapsedSeconds), 0)*(level+1);
    incrementScoreByAmount(scoreToAdd);
}

//Adds the given amount to the score counter
void Stage3Dialog::incrementScoreByAmount(int amount) {
    int i = 0;
    while(i++ < amount) score.increment();
}

//Shows the end of game message with final score and restart info
void Stage3Dialog::renderEndOfGameMessage(Renderer &renderer) {
    QPainter &painter = renderer.getPainter();
    QPen pen = painter.pen();
    QFont font = painter.font();
    font.setPointSize(50);
    pen.setColor(Qt::darkGray);
    painter.setPen(pen);
    painter.setFont(font);
    string endGameText = "Game Over";
    if(level + 1 == levelLayouts.size() && obstaclesAdded != 0 && obstaclesAdded == obstaclesOffScreen) endGameText = "You Win!";
    painter.drawText(0,25, 800, 100, Qt::AlignHCenter, QString::fromStdString(endGameText));
    font.setPointSize(25);
    painter.setFont(font);
    std::string finalScoreStr = std::to_string(score.getScore());
    painter.drawText(0,100, 800, 50, Qt::AlignHCenter, QString::fromStdString("Final Score: " + finalScoreStr));
    painter.drawText(0,0, 800, 50, Qt::AlignHCenter, "Press R to Restart from Level 1");
    leaderboard.renderLeaderboard(renderer);
}

// Implementation of subject interface. Figures out who triggered the update
// and what action to now take based on event struct member values
void Stage3Dialog::update(shared_ptr<const Event> evt) {
    Event eventDeRef = *evt;
    if(eventDeRef.producer.compare("jumpingStickmanSubject") == 0
        && eventDeRef.event.compare("collided") == 0)
    {
        if(size.compare("star") != 0) {
            updateIsColliding();
        } else {
            obstaclesOffScreen++;
            auto it = obstacles.begin() + eventDeRef.index;
            obstacles.erase(it);
        }

    //hit a bonus item - powerup, coin, heart
    } else if (eventDeRef.event.compare("bonus") == 0) {

        auto it = obstacles.begin() + eventDeRef.index;
        obstacles.erase(it);
        if (eventDeRef.name.compare("coin") == 0) {
            incrementScoreByAmount(10);
        } else if (eventDeRef.name.compare("heart") == 0) {
            lives++;
            decoratorObserver->updateLives(lives);
        //hit a powerup - set size information to handle collisions
        } else {
            size = eventDeRef.name;
        }
    }
}

// Needs to be overriden to prevent deletion of the obstacles
// We rather keep track of them until the end of the level
// to enable backwards movement
void Stage3Dialog::renderObstacles(Renderer &renderer, unsigned int counter) {
    for (auto &o: obstacles) {
        o->render(renderer, counter);
    }
}

// For off-screen object tracking - checks if the entity is
// one that we could collide with as that determines
// when the level is over
bool Stage3Dialog::isObstacle(shared_ptr<Entity> entity) {
    string name = entity->getName();
    return (name.compare("coin") != 0
            && name.compare("heart") != 0
            && name.compare("bigger") != 0
            && name.compare("smaller") != 0
            && name.compare("star") != 0
            && name.compare("normal") != 0);
}
