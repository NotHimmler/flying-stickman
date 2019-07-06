#include "stagefactory.h"
#include "collisiontest.h"
#include "jumptest.h"
#include "flyingobstacletest.h"
#include "swaprendererstage.h"
#include "testingdialog.h"
#include "stage2dialog.h"
#include "stage3dialog.h"
#include "dialog.h"
#include "moon.h"
#include "background.h"
#include "jumpingstickmansubject.h"
#include <iostream>

using namespace std;

StageFactory::StageFactory(Config config) : config(config) {

}

std::shared_ptr<GameStage> StageFactory::createStage() {
    if (config.stage == 3) {
        // Stage 3 non-test mode
        config.velocity = 0;
        auto player = std::make_shared<JumpingStickmanSubject>(config.coord.getYCoordinate());

        player->setSize(config.size);
        player->setCoordinate(config.coord);
        player->setSprite(":sprites/sprite0.png");

        auto factory = std::make_shared<EntityFactory>();
        factory->setVelocity(config.velocity);
        auto stage = std::make_shared<Stage3Dialog>(*config.game, player, std::move(factory), std::move(*config.levelObstacles), config.lives);


        genericDialogInitializer(*stage);

        auto stageDecorator = std::make_shared<Stage3Decorator>(stage, config.lives);

        stage->addObserver(stageDecorator);
        player->addObserver(stageDecorator);
        player->addObserver(stage);
        return stageDecorator;
    } else if (config.stage == 2) {
        if (config.testMode) {
            // Stage 2 test mode
            std::vector<std::shared_ptr<TestRunner>> tests;
            tests.push_back(std::make_shared<CollisionTest>());
            tests.push_back(std::make_shared<JumpTest>());
            tests.push_back(std::make_shared<FlyingObstacleTest>());

            std::shared_ptr<GameStage> tester = std::make_shared<TestingDialog>(std::move(tests));
            return std::make_shared<SwapRendererStage>(std::move(tester));
        } else {
            // Stage 2 non-test mode
            auto player = std::make_shared<JumpingStickman>(config.coord.getYCoordinate());
            player->setSize(config.size);
            player->setCoordinate(config.coord);
            player->setSprite(":sprites/sprite0.png");

            auto factory = std::make_shared<EntityFactory>();
            factory->setVelocity(config.velocity);

            auto stage = std::make_shared<Stage2Dialog>(*config.game, std::move(player), std::move(factory), std::move(*config.obstacles));
            genericDialogInitializer(*stage);
            return std::make_shared<SwapRendererStage>(std::move(stage));
        }
    } else {
        // Stage 1
        auto player = std::make_shared<Stickman>();
        player->setSize(config.size);
        player->setCoordinate(config.coord);
        player->setSprite(":sprites/sprite0.png");

        auto factory = std::make_shared<EntityFactory>();
        factory->setVelocity(config.velocity);

        auto stage = std::make_shared<Dialog>(*config.game, std::move(player), std::move(factory));
        genericDialogInitializer(*stage);
        return std::move(stage);
    }
}

// Avoid duplicating the same common initialization code across stage 1, 2 and (probably) 3
void StageFactory::genericDialogInitializer(Dialog &dialog) {
    Background bg;
    bg.setCoordinate(Coordinate(0, 150, 450));
    bg.setSprite(config.background);
    bg.setVelocity(config.velocity);

    dialog.setBackground(bg);
    dialog.setMoon(Moon(Coordinate(400, -140, 450), 181.0, 550));
}
