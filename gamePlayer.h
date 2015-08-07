#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <QKeyEvent>
#include <QVector3D>
#include "gamePlayer.h"
#include "energyball.h"
#include "entity.h"

class Entity;

class GamePlayer
{
public:
    GamePlayer( Entity * entity, char scenarioID );
    ~GamePlayer();

    void update( float elapsedTime );

    void startMoveUp();
    void startMoveLeft();
    void startMoveRight();
    void startMoveDown();

    void stopMoveUp();
    void stopMoveLeft();
    void stopMoveRight();
    void stopMoveDown();

    void buffEnergyPower();
    void buffWalkSpeed();
    void buffEnergyCoolDown();

    EnergyBall * castEnergyBall();

    float energyPower;
    float walkSpeed;
    float energyCoolDown;

    float cdPerSecond;

    Entity * entity;
    char scenarioID;
private:
    QVector3D faceDirection;


    int movingKey;

};

#endif // GAME_PLAYER_H
