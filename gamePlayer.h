#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <QKeyEvent>
#include "gamePlayer.h"
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

private:
    Entity * entity;

    char scenarioID;
    int movingKey;
};

#endif // GAME_PLAYER_H
