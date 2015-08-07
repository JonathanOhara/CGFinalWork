#ifndef ENERGYBALL_H
#define ENERGYBALL_H

#include <QVector3D>
#include <QMatrix4x4>

class Entity;
class GamePlayer;

class EnergyBall
{
public:
    EnergyBall( QVector3D direction, QVector3D position, float energyPower, GamePlayer * caster );
    ~EnergyBall( );

    void update( float elasedTime );

    void reScale();

    Entity * entity;
    float energyPower;
    GamePlayer * caster;
private:
    QVector3D direction;
    QVector3D position;

    QMatrix4x4 initialTransformation;
    float speed;

};

#endif // ENERGYBALL_H
