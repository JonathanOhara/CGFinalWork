#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <QKeyEvent>
#include "gamePlayer.h"
#include "entity.h"

class Entity;

class GamePlayer
{
public:
    GamePlayer( Entity * entity );
    ~GamePlayer();


    void update( float elapsedTime );

    void keyPressEvent( QKeyEvent * event );
    void keyReleaseEvent( QKeyEvent * event );

private:
    Entity * entity;

    int movingKey;
};

#endif // GAME_PLAYER_H
