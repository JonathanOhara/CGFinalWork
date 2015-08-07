#ifndef SCENARIO_H
#define SCENARIO_H

#define LINES 13
#define COLUMNS 15

#include <math.h>
#include <string>
#include <QDebug>

class GamePlayer;

class Scenario
{
public:
    static char cenario[LINES][COLUMNS];

    static bool canMoveUp( GamePlayer *player, float x, float y, float z );
    static bool canMoveLeft( GamePlayer *player, float x, float y, float z );
    static bool canMoveRight( GamePlayer *player, float x, float y, float z );
    static bool canMoveDown( GamePlayer *player, float x, float y, float z );

    static void updateScenario( GamePlayer *player, float x, float y, float z );

    static void destroyBlock( GamePlayer *player,float x, float y, float z );
    static void destroyPowerUp( GamePlayer *player,float x, float y, float z );

private:
    static int* findPosition( char id );
    static bool isPowerUp( char c );
    static void print();
};

#endif // SCENARIO_H
