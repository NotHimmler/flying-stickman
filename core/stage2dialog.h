#ifndef STAGE2DIALOG_H
#define STAGE2DIALOG_H

#include "dialog.h"

class Stage2Dialog : public Dialog {
public:
    /**
     * @brief Stage2Dialog
     * @param game The container of the dialog
     * @param stickman The player's stickman object
     * @param factory An entity factory for producing obstacles
     * @param obstacleLayout Pairs of (obstacle, space_until_next_obstacle) describing the sequence in which obstacles will be spawned
     */
    Stage2Dialog(Game &game, std::shared_ptr<Stickman> stickman, std::shared_ptr<EntityFactory> factory, std::vector<std::pair<std::shared_ptr<Entity>, int>> obstacleLayout);

    void update();

protected:
    virtual void spawnObstacles(unsigned int counter);

private:
    std::vector<std::pair<std::shared_ptr<Entity>, int>> obstacleLayout;
    int distanceToSpawn;
    int nextObstacle;
};

#endif // STAGE2DIALOG_H
