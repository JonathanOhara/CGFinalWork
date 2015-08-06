#include "scenario.h"

char Scenario::cenario[LINES][COLUMNS] = {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X','.','H','.','B','B','B','.','B','B','B','.','.','.','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','B','.','B','B','B','B','B','B','B','.','B','B','X'},
    {'X','B','X','B','X','.','X','.','X','.','X','B','X','B','X'},
    {'X','B','B','B','B','B','B','.','B','B','B','B','B','B','X'},
    {'X','.','X','B','X','.','X','.','X','.','X','B','X','.','X'},
    {'X','B','B','B','B','B','B','.','B','B','B','B','B','B','X'},
    {'X','B','X','B','X','.','X','.','X','.','X','B','X','B','X'},
    {'X','.','B','.','B','B','B','B','B','B','B','.','B','.','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','.','.','B','B','B','.','B','B','B','.','.','E','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
};

bool Scenario::canMoveUp( const char id, float x, float y, float z){
    int * position = findPosition(id);

    int line = trunc(z + LINES);
    int column = position[1];

    delete[] position;

    if( z + LINES == line){
        if( Scenario::cenario[ line - 1 ][ column ] == '.' ){
            return true;
        }
    }else{
        if( Scenario::cenario[ line ][ column ] == '.' || Scenario::cenario[ line ][ column ] == id ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveLeft(const char id, float x, float y, float z){
    int * position = findPosition(id);


    int line = position[0];
    int column = trunc(x);
    delete[] position;

    if( x == column ){
        if( Scenario::cenario[ line ][ column -1 ] == '.' ){
            return true;
        }
    }else{
        if( Scenario::cenario[ line ][ column ] == '.' || Scenario::cenario[ line ][ column ] == id ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveRight( const char id, float x, float y, float z){
    int * position = findPosition(id);

    int line = position[0];
    int column = ceil(x);
    delete[] position;


    if( x == column ){
        if( Scenario::cenario[ line ][ column +1 ] == '.' ){
            return true;
        }
    }else{
        if( Scenario::cenario[ line ][ column ] == '.' || Scenario::cenario[ line ][ column ] == id ){
            return true;
        }
    }

    return false;
}

bool Scenario::canMoveDown(const char id, float x, float y, float z){
    int * position = findPosition(id);

    int line = ceil(z + LINES);
    int column = position[1];
    delete[] position;

    if( z + LINES == line){
        if( Scenario::cenario[ line + 1 ][ column ] == '.' ){
            return true;
        }
    }else{
        if( Scenario::cenario[ line ][ column ] == '.' || Scenario::cenario[ line ][ column ] == id ){
            return true;
        }
    }

    return false;
}

void Scenario::updateScenario(const char id, float x, float y, float z){
    int * position = findPosition(id);

    int line = position[0];
    int column = position[1];
    delete[] position;

    if( Scenario::cenario[line][column] == '.' || Scenario::cenario[line][column] == id ){

        int newLine = line;
        int newCoulmn = column;

        newCoulmn = newCoulmn + ( round(x) - column );
        newLine = newLine + ( round(z) + LINES - line );

        Scenario::cenario[line][column] = '.';
        Scenario::cenario[newLine][newCoulmn] = id;
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
