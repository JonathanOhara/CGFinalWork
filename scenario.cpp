#include "scenario.h"
#include "gamePlayer.h"

char Scenario::cenario[LINES][COLUMNS] = {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X','H','.','.','B','B','B','C','B','B','B','S','.','P','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','B','C','B','B','B','B','B','B','B','.','B','B','X'},
    {'X','B','X','B','X','P','X','S','X','S','X','B','X','B','X'},
    {'X','B','B','B','B','B','B','.','B','B','B','B','B','B','X'},
    {'X','.','X','B','X','P','X','P','X','P','X','B','X','.','X'},
    {'X','B','B','B','B','B','B','.','B','B','B','B','B','B','X'},
    {'X','B','X','B','X','S','X','S','X','P','X','B','X','B','X'},
    {'X','.','B','.','B','B','B','B','B','B','B','C','B','.','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','P','.','S','B','B','B','C','B','B','B','.','.','E','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
};

bool Scenario::canMoveUp( GamePlayer *player, float x, float y, float z){
    int * position = findPosition( player->scenarioID );

    int line = trunc(z + LINES);
    int column = position[1];

    delete[] position;

    char c;
    if( z + LINES == line){
        c = Scenario::cenario[ line - 1 ][ column ];
        if( c == '.' || isPowerUp(c) ){
            return true;
        }
    }else{
        c = Scenario::cenario[ line ][ column ];
        if( c == '.' || isPowerUp(c) || c == player->scenarioID ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveLeft(GamePlayer *player, float x, float y, float z){
    int * position = findPosition( player->scenarioID );


    int line = position[0];
    int column = trunc(x);
    delete[] position;

    char c;
    if( x == column ){
        c = Scenario::cenario[ line ][ column -1 ];
        if( c == '.' || isPowerUp(c)  ){
            return true;
        }
    }else{
        c = Scenario::cenario[ line ][ column ];
        if( c == '.' || isPowerUp(c)  || c == player->scenarioID ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveRight( GamePlayer *player, float x, float y, float z){
    int * position = findPosition( player->scenarioID );

    int line = position[0];
    int column = ceil(x);
    delete[] position;


    char c;
    if( x == column ){
        c = Scenario::cenario[ line ][ column +1 ];
        if( c == '.' || isPowerUp(c)  ){
            return true;
        }
    }else{
        c = Scenario::cenario[ line ][ column ];
        if( c == '.' || isPowerUp(c)  || c == player->scenarioID ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveDown(GamePlayer *player, float x, float y, float z){
    int * position = findPosition( player->scenarioID );

    int line = ceil(z + LINES);
    int column = position[1];
    delete[] position;

    char c;
    if( z + LINES == line){
        c = Scenario::cenario[ line + 1 ][ column ];
        if( c == '.' || isPowerUp(c)  ){
            return true;
        }
    }else{
        c = Scenario::cenario[ line ][ column ];
        if( c == '.' || isPowerUp(c)  || c == player->scenarioID ){
            return true;
        }
    }

    return false;
}

void Scenario::destroyBlock(GamePlayer *player, float x, float y, float z){
    int line = (int)(z + LINES);
    int column = (int)x;

    Scenario::cenario[ line ][ column ] = '.';

    //POWER UP CHANCE
}

void Scenario::destroyPowerUp(GamePlayer *player, float x, float y, float z){
    int line = (int)(z + LINES);
    int column = (int)x;

    Scenario::cenario[ line ][ column ] = player->scenarioID;

    //POWER UP CHANCE
}

void Scenario::updateScenario(GamePlayer *player, float x, float y, float z){
    int * position = findPosition( player->scenarioID );

    int line = position[0];
    int column = position[1];
    delete[] position;

    if( Scenario::cenario[line][column] == '.' || Scenario::cenario[line][column] == player->scenarioID ){

        int newLine = line;
        int newCoulmn = column;

        newCoulmn = newCoulmn + ( round(x) - column );
        newLine = newLine + ( round(z) + LINES - line );

        Scenario::cenario[line][column] = '.';
        Scenario::cenario[newLine][newCoulmn] = player->scenarioID;
    }
}

int* Scenario::findPosition(char id){
    int* position = new int[2];

    //qDebug() << " id = " << id;

    for( int i = 0; i < LINES; i++ ){
        for( int j = 0; j < COLUMNS; j++){

            if( id == Scenario::cenario[i][j] ){
                position[0] = i;
                position[1] = j;

                return position;
            }
        }
    }


    return position;
}

bool Scenario::isPowerUp(char c){
    return c == 'P' || c == 'C' || c == 'S';
}

void Scenario::print(){
    qDebug() << "print()";
    std::string str;
    for( int i = 0; i < LINES; i++ ){
        for( int j = 0; j < COLUMNS; j++){
            str += Scenario::cenario[i][j];
            str.append(", ");
        }
        str.append("\n");
    }
    qDebug() <<  str.c_str();
}
