#include "gamePlayer.h"
#include "scenario.h"
#include "math.h"

GamePlayer::GamePlayer( Entity * entity, char scenarioID ){
    this->entity = entity;
    this->scenarioID = scenarioID;

    energyCoolDown = 0;
    movingKey = 0;

    energyPower = 1;
    walkSpeed = 6;
    cdPerSecond = 20;

    if( scenarioID == 'H' ){
        faceDirection = QVector3D( 0, 0, 1 );
    }else{
        faceDirection = QVector3D( 0, 0, -1 );
    }
}

GamePlayer::~GamePlayer(){

}

void GamePlayer::startMoveUp(){
    entity->clearRotation();
    movingKey = Qt::Key_Up;

    faceDirection = QVector3D( 0, 0, -1 );
}

void GamePlayer::startMoveLeft(){
    entity->clearRotation();
    entity->rotate(90, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Left;

    faceDirection = QVector3D( -1, 0, 0 );
}

void GamePlayer::startMoveRight(){
    entity->clearRotation();
    entity->rotate(270, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Right;

    faceDirection = QVector3D( 1, 0, 0 );
}

void GamePlayer::startMoveDown(){
    entity->clearRotation();
    entity->rotate(180, QVector3D( 0, 1, 0 ) );
    movingKey = Qt::Key_Down;

    faceDirection = QVector3D( 0, 0, 1 );
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

void GamePlayer::buffEnergyPower(){
    energyPower = min( 5.0f, energyPower + 1);
}

void GamePlayer::buffWalkSpeed(){
    walkSpeed = min( 18.0f, walkSpeed + 3);
}

void GamePlayer::buffEnergyCoolDown(){
    cdPerSecond = min( 50.0f, cdPerSecond + 5);
}

void GamePlayer::update(float elapsedTime){
    switch( movingKey ){
    case Qt::Key_Left:
        if( Scenario::canMoveLeft( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundZ();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Right:
        if( Scenario::canMoveRight( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundZ();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Up:
        if( Scenario::canMoveUp( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundX();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;

    case Qt::Key_Down:
        if( Scenario::canMoveDown( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() ) ){
            entity->roundX();
            entity->translate( QVector3D( 0, 0, walkSpeed * -1 *elapsedTime ) );
            Scenario::updateScenario( this, entity->getPosition().x(), entity->getPosition().y(), entity->getPosition().z() );
        }
    break;
    }

    float stopBonus = movingKey == 0 ? cdPerSecond * 0.10 : 0;
    energyCoolDown = min( 100.0f, energyCoolDown + (cdPerSecond * elapsedTime) + ( stopBonus * elapsedTime ) );
}

EnergyBall * GamePlayer::castEnergyBall(){
    EnergyBall * energyBall = NULL;

    if( energyCoolDown == 100 ){
        energyCoolDown = 0;
        energyBall = new EnergyBall( faceDirection, entity->getPosition(), energyPower, this );
    }

    return energyBall;
}
