#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <fstream>
#include <limits>
#include <iostream>
#include <string>


#include "light.h"
#include "material.h"
#include "camera.h"
#include "ConfigLoader.hpp"
#include "rapidxml.hpp"
#include "gamePlayer.h"
#include "entity.h"
#include "scenario.h"

class OpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
Q_OBJECT

public :
    explicit OpenGLWidget ( QWidget * parent = 0) ;
    ~OpenGLWidget () ;
/*
    void destroyVBOs();
    void createVBOs();

    void readOFFFile(const QString &fileName);
    void calculateNormals();

    void genTexCoordsCylinder();
    void genTangents();
*/
    void keyPressEvent( QKeyEvent * event );
    void keyReleaseEvent( QKeyEvent * event );

    void mouseMoveEvent( QMouseEvent * event );
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );

protected:
    void initializeGL () ;
    void resizeGL ( int width , int height ) ;
    void paintGL () ;

    void loadLevel();

    void gameLogic( float deltaTime );

private:
    QTimer timer;
    QElapsedTimer elapsedTime;

    Camera camera;
    Light light;
    Material material;

    QVector4D ambientProduct;
    QVector4D diffuseProduct;
    QVector4D specularProduct;

    QMatrix4x4 modelView;
    QMatrix4x4 projectionMatrix;

    std::list<Entity*> objects;

    GamePlayer *player1, *player2;

signals :
    void statusBarMessage ( QString ns ) ;

public slots :
    /*
    void createShaders () ;
    void destroyShaders () ;

    void createTexture();
    void createTextureLayer();

    void toggleBackgroundColor ( bool changeBColor ) ;
    void toggleDiagonal ( bool diagonal );
    void showFileOpenDialog() ;
*/
    void animate();
};
# endif // OPENGLWIDGET_H
