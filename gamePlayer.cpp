#include "gamePlayer.h"

GamePlayer::GamePlayer( Entity * entity ){
    this->entity = entity;

    movingKey = 0;
}

GamePlayer::~GamePlayer(){

}


void GamePlayer::update(float elapsedTime){
//    qDebug() << "elapsed time " << elapsedTime;
    switch( movingKey ){
    case Qt::Key_Left:
        entity->translate( QVector3D( 0, 0, -1 *elapsedTime ) );
    break;

    case Qt::Key_Right:
        entity->translate( QVector3D( 1 *elapsedTime, 0, 0 ) );
    break;

    case Qt::Key_Up:
        entity->translate( QVector3D( 0, 0, 1 *elapsedTime ) );
    break;

    case Qt::Key_Down:
        entity->translate( QVector3D( -1 *elapsedTime, 0, 0 ) );

    break;
    }
}

void GamePlayer::keyPressEvent( QKeyEvent * event ){
    switch(event->key()){
        case Qt::Key_Left:
            movingKey = event->key();
        break;

        case Qt::Key_Right:
            movingKey = event->key();
        break;

        case Qt::Key_Up:
            movingKey = event->key();
        break;

        case Qt::Key_Down:
            movingKey = event->key();
        break;
    }
}

void GamePlayer::keyReleaseEvent( QKeyEvent * event ){
    switch(event->key()){
        case Qt::Key_Left:
            movingKey = 0;
        break;

        case Qt::Key_Right:
            movingKey = 0;
        break;

        case Qt::Key_Up:
            movingKey = 0;
        break;

        case Qt::Key_Down:
            movingKey = 0;
        break;
    }
}
