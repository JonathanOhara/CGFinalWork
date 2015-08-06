#include "gamePlayer.h"
#include "scenario.h"

GamePlayer::GamePlayer( Entity * entity, char scenarioID ){
    this->entity = entity;
    this->scenarioID = scenarioID;

    movingKey = 0;
}

GamePlayer::~GamePlayer(){

}

void GamePlayer::startMoveUp(){
    entity->clearRotation();
    movingKey = Qt::Key_Up;
}

void GamePlayer::startMoveLeft(){
    entity->clearRotation();
    entity->rotate(90, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Left;
}

void GamePlayer::startMoveRight(){
    entity->clearRotation();
    entity->rotate(270, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Right;
}

void GamePlayer::startMoveDown(){
    entity->clearRotation();
    entity->rotate(180, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Down;
}


void GamePlayer::stopMoveUp(){
    if( movingKey == Qt::Key_Up ) movingKey = 0;
}

void GamePlayer::stopMoveLeft(){
    if( movingKey == Qt::Key_Left ) movingKey = 0;
}

void GamePlayer::stopMoveRight(){
    if( movingKey == Qt::Key_Right ) movingKey = 0;
}

void GamePlayer::stopMoveDown(){
    if( movingKey == Qt::Key_Down ) movingKey = 0;
}

void GamePlayer::update(float elapsedTime){
    int walkSpeed = 8;

    switch( movingKey ){
    case Qt::Key_Left:
        if( Scenario::canMoveLeft( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundZ();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Right:
        if( Scenario::canMoveRight( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundZ();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Up:
        if( Scenario::canMoveUp( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundX();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Down:
        if( Scenario::canMoveDown( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundX();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( scenarioID, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;
    }
}
