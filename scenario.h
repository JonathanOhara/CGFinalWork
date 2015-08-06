#ifndef SCENARIO_H
#define SCENARIO_H

#define LINES 13
#define COLUMNS 15

#include <math.h>
#include <string>
#include <QDebug>

class Scenario
{
public:
    static char cenario[LINES][COLUMNS];

    static bool canMoveUp( const char id, float x, float y, float z );
    static bool canMoveLeft( const char id, float x, float y, float z );
    static bool canMoveRight( const char id, float x, float y, float z );
    static bool canMoveDown( const char id, float x, float y, float z );

    static void updateScenario( const char id, float x, float y, float z );

private:
    static int* findPosition( char id );
    static void print();
};

#endif // SCENARIO_H
