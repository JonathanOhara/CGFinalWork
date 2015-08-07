#include "energyball.h"
#include "entity.h"
#include "gamePlayer.h"

EnergyBall::EnergyBall(QVector3D direction, QVector3D position, float energyPower, GamePlayer * caster){
    this->direction = direction;
    this->position = position;
    this->energyPower = energyPower;
    this->caster = caster;

    entity = new Entity( "energy_ball" , "bomb.mesh.xml");
    entity->setPosition( QVector3D( round( this->position.x() ), 0.25f, round( this->position.z() ) ) );

    if( direction.x() == 1 ){
        entity->rotate(270, QVector3D( 0, 1, 0 ) );
    }else if( direction.x() == -1 ){
        entity->rotate(90, QVector3D( 0, 1, 0 ) );
    }else if( direction.z() == -1 ){
        entity->clearRotation();
    }else if( direction.z() == 1 ){
        entity->rotate(180, QVector3D( 0, 1, 0 ) );
    }

    initialTransformation = entity->getTransformation();

    entity->setScale( QVector3D(0.1f * energyPower, 0.1f * energyPower, 0.1f * energyPower) );

    speed = 18;
}

EnergyBall::~EnergyBall(){

}
void EnergyBall::reScale(){
    QVector3D pos = entity->getPosition();
    entity->setTransformation(initialTransformation);
    entity->setPosition(pos);
    entity->setScale( QVector3D(0.1f * energyPower, 0.1f * energyPower, 0.1f * energyPower) );
}

void EnergyBall::update(float elasedTime){
    float step = speed * elasedTime;

    entity->translate( QVector3D(0, 0, step* -1) );
}
