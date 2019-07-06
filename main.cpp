#include "dialog.h"
#include <QApplication>

#include <iostream>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <time.h>
#include <vector>
#include <tuple>
#include "entityfactory.h"
#include "jumpingstickman.h"
#include "colouredentity.h"
#include "flyingentity.h"
#include "stage2dialog.h"
#include "testrunner.h"
#include "testingdialog.h"
#include "collisiontest.h"
#include "jumptest.h"
#include "swaprendererstage.h"
#include "flyingobstacletest.h"
#include "stagefactory.h"
#include <iostream>
using namespace std;

inline bool exists(const std::string& name) {
    QFileInfo fileInfo(name.c_str());
    std::cout << name << std::endl;
    return fileInfo.exists();
}

bool isNumber(string value) {
    for (size_t i = 0; i < value.length(); i++) {
        if(!isdigit(value[i])) {
            return false;
        }
    }
    return true;
}

// A better version of the above method, but I don't want to remove it in case I lose marks
bool isNumber(QString value) {
    bool ok;
    value.toInt(&ok);
    return ok;
}

int main(int argc, char *argv[]) {
    srand(time(nullptr));

    QApplication a(argc, argv);

    Game game;
    int stage;
    cout << "Width: " << game.width() << ", Height: " << game.height() << endl;
    // Set up background
    Background background;
    background.setSprite(":ground.png");
    background.setCoordinate(Coordinate(0, 150, 450));

    StageFactory::Config stageConfig;
    vector<vector<pair<shared_ptr<Entity>, int>>> obstaclesVector;
    //stageConfig.obstacles = &obstacles;
    stageConfig.game = &game;
    stageConfig.lives = 3;
    // Read config file and set basic game attributes
    QString fileName = "../Assignment3/config.txt";
    if (!exists(fileName.toStdString())) {
        //Default config file not found, select one from FS
        fileName = QFileDialog::getOpenFileName(nullptr, ("Open Config"), QString(), ("Text Files (*.txt)"));


        if(fileName.compare("") == 0) {
            cout << "No config file specified or found. Terminating." << endl;
            return 0;
        }
    }

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);
    QTextStream stream(&inputFile);
    QString line = stream.readLine();
    while (!line.isNull()) {
        QStringList args = line.split(" ");
        if (args.length() != 2) {
            cout << "Invalid config format. Terminating" << endl;
            return 0;
        }
        string setting = line.split(" ").at(0).toStdString();
        QString value = line.split(" ").at(1);
        if (setting == "size:") {
            std::string size = value.toStdString();
            if (size.compare("tiny") != 0 &&
                    size.compare("normal") != 0 &&
                    size.compare("large") != 0 &&
                    size.compare("giant") != 0) {
                cout << "Size must be tiny, normal, large or giant. Terminating" << endl;
                return 0;
            }
            stageConfig.size = size;
        } else if (setting == "x:") {
            if (!isNumber(value.toStdString())) {
                cout << "X coordinate must be set to a postive integer. Terminating";
                return 0;
            }
            unsigned int x = value.toUInt();
            if (x > 800) {
                cout << "X coordinate must be between 0 and 800. Terminating";
                return 0;
            }
            stageConfig.coord = Coordinate(x, 150, 450);
        } else if (setting == "velocity:") {
            if (!isNumber(value.toStdString())) {
                cout << "Velocity must be set to a positive integer. Terminating";
                return 0;
            }
            stageConfig.velocity = value.toInt();
        } else if (setting == "background:") {
            string path = value.toStdString();
            if (exists(path)) {
                stageConfig.background = path;
            } else {
                cout << "File \"" + path + "\" not found. Terminating";
                return 0;
            }
        } else if (setting == "stage:") {
            if (!isNumber(value.toStdString())) {
                cout << "Stage value must be a positive integer. Terminating";
                return 0;
            }
            stage = value.toInt();
            stageConfig.stage = value.toInt();
            if (stageConfig.stage != 1 && stageConfig.stage != 2 && stageConfig.stage != 3) {
                cout << "Invalid stage value. Terminating";
                return 0;
            }
        } else if (setting == "obstacles:") {
            QStringList parts = value.split("|");
            EntityFactory factory;
            vector<pair<shared_ptr<Entity>, int>> obstacles;
            for (QString &s : parts) {
                QStringList sl = s.split(",");

                // Make sure each obstacle config has 8 integer parameters
                bool ok = true;
                for (int i = 0; i < sl.length() && ok; i++) {
                    ok = isNumber(sl.at(i));
                }
                if (sl.length() != 8 || !ok) {
                    cout << "Invalid obstacle values. Terminating.";
                    return 0;
                }

                // Make a coloured bird obstacle according to the config
                auto e = factory.getEntity("bird");
                e = make_shared<ColouredEntity>(move(e), QColor(sl.at(4).toInt(), sl.at(5).toInt(), sl.at(6).toInt()));
                e->setSize(sl.at(0).toInt(), sl.at(1).toInt());
                e->getCoordinate().setYCoordinate(sl.at(2).toInt());
                int flyRate = sl.at(7).toInt();
                if (flyRate != 0) {
                    e = make_shared<FlyingEntity>(move(e), flyRate);
                }

                // Add the pair (obstacle, spacing_to_next_obstacle) to our obstacle layout
                obstacles.push_back(make_pair(move(e), sl.at(3).toInt()));
            }
            obstaclesVector.push_back(obstacles);
        } else if (setting == "testMode:") {
            stageConfig.testMode = value.compare("on") == 0;
        } else if (setting == "lives:") {
            if (!isNumber(value.toStdString()) || value.toInt() < 1) {
                cout << "Lives value must be a positive integer. Terminating" << endl;
                return 0;
            }
            stageConfig.lives = value.toInt();

        }
        line = stream.readLine();
    };

    if(obstaclesVector.size() > 0) {
        stageConfig.obstacles = &(obstaclesVector.front());
    } else {
        std::vector<pair<shared_ptr<Entity>, int>> obstacles;
        stageConfig.obstacles = &obstacles;
    }

    stageConfig.levelObstacles = &obstaclesVector;
    // Construct and set stage
    game.setStage(StageFactory(stageConfig).createStage());
    game.show();

    return a.exec();
}
