#include "openglwidget.h"

#define LINES 13
#define COLUMNS 15

OpenGLWidget::OpenGLWidget ( QWidget * parent ):  QOpenGLWidget ( parent ) {
    player1 = NULL;
    player2 = NULL;
}

OpenGLWidget::~OpenGLWidget (){
    std::list<Entity*>::iterator object = objects.begin();
    Entity * obj;

    while( object != objects.end() ){
        obj = (*object);

        delete obj;
    }

    if( player1 != NULL ) delete player1;
    if( player2 != NULL ) delete player2;
}

void OpenGLWidget :: initializeGL () {
    initializeOpenGLFunctions () ;
    std :: cerr << " Version " << glGetString ( GL_VERSION ) <<"\n";
    std :: cerr << " GLSL " << glGetString ( GL_SHADING_LANGUAGE_VERSION )<<"\n";
    glEnable ( GL_DEPTH_TEST );

    connect (&timer , SIGNAL ( timeout () ) , this , SLOT ( animate () ) ) ;

    loadLevel();

    timer.start (0) ;
    elapsedTime.start();
}

void OpenGLWidget :: resizeGL (int w , int h ) {
    glViewport (0 , 0 , w , h ) ;

    projectionMatrix . setToIdentity () ;
    projectionMatrix . perspective (60.0 , static_cast < qreal >( w ) / static_cast < qreal >( h ) , 0.1 , 100.0) ;

    update();
}

void OpenGLWidget :: paintGL () {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

//    qDebug() << "paintGL 1";

    if( objects.size() == 0 ) return;

    std::list<Entity*>::iterator object = objects.begin();

//    qDebug() << "paintGL 2";
    Entity * obj;


    modelView.setToIdentity () ;
    modelView.lookAt ( camera.eye , camera.at , camera.up ) ;

//    qDebug() << "paintGL 3";

    QMatrix4x4 modelViewMatrix;

    while( object != objects.end() ){
        obj = (*object);

//        qDebug() << "paintGL 4";

        obj->getShaderProgram()->bind();
//        qDebug() << "paintGL 5";

        ambientProduct = light.ambient * obj->getMaterial()->ambient ;
        diffuseProduct = light.diffuse * obj->getMaterial()->diffuse ;
        specularProduct = light.specular * obj->getMaterial()->specular ;

//        qDebug() << "paintGL 6";

        modelViewMatrix = modelView * obj->getTransformation();

        obj->getShaderProgram()->setUniformValue("lightPosition", light.position ) ;
        obj->getShaderProgram()->setUniformValue("ambientProduct", ambientProduct ) ;
        obj->getShaderProgram()->setUniformValue("diffuseProduct", diffuseProduct ) ;
        obj->getShaderProgram()->setUniformValue("specularProduct", specularProduct ) ;
        obj->getShaderProgram()->setUniformValue("shininess", static_cast < GLfloat >( material.shininess ) ) ;

        obj->getShaderProgram()->setUniformValue("modelView", modelViewMatrix ) ;
        obj->getShaderProgram()->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix() ) ;
        obj->getShaderProgram()->setUniformValue("projectionMatrix", projectionMatrix );

//        qDebug() << "paintGL 7";

        obj->getVboVertices()-> bind () ;
        obj->getShaderProgram() -> enableAttributeArray ("vPosition") ;
        obj->getShaderProgram() -> setAttributeBuffer ("vPosition", GL_FLOAT , 0 , 4 , 0) ;

//        qDebug() << "paintGL 8";

        obj->getVboNormals()->bind();
        obj->getShaderProgram() -> enableAttributeArray ("vNormal") ;
        obj->getShaderProgram() -> setAttributeBuffer ("vNormal", GL_FLOAT , 0 , 3 , 0) ;

//        qDebug() << "paintGL 9";

        obj->getVboTangents()->bind();
        obj->getShaderProgram()->enableAttributeArray ("vTangent") ;
        obj->getShaderProgram()->setAttributeBuffer ("vTangent", GL_FLOAT , 0 , 4 , 0) ;

//        qDebug() << "paintGL 10";

        obj->getVboIndices()->bind() ;

//        qDebug() << "paintGL 11";

        obj->getVbocoordText() -> bind ();
        obj->getShaderProgram() -> enableAttributeArray ("vcoordText") ;
        obj->getShaderProgram() -> setAttributeBuffer ("vcoordText", GL_FLOAT , 0 , 2 , 0) ;

//        qDebug() << "paintGL 12";

        obj->getMaterial()->texture -> bind (0) ;
        obj->getShaderProgram() -> setUniformValue ("colorTexture", 0) ;

//        qDebug() << "paintGL 13";

        glDrawElements ( GL_TRIANGLES , obj->getFacesCount() * 3 , GL_UNSIGNED_INT , 0) ;
//       qDebug() << "paintGL 14";

        obj->getShaderProgram()->release();
        obj->getVboVertices()->release();
        obj->getVboNormals()->release();
        obj->getVboIndices()->release();
        obj->getVbocoordText()->release();
        obj->getMaterial()->texture->release(0);

//        qDebug() << "paintGL 15";

        object++;
    }
//    qDebug() << "paintGL 16";

    float deltaTime = (float) elapsedTime.elapsed() / 1000;
    gameLogic( deltaTime );
    elapsedTime.restart();
}


void OpenGLWidget :: animate (){
    update () ;
}

//----------------------------------------------------------------------------------------------
void OpenGLWidget::keyPressEvent( QKeyEvent * event ){
    switch(event->key()){
        case Qt::Key_Escape:
            qApp->quit();
        break;

        case Qt::Key_Q:
            camera.eye.setX( camera.eye.x() - 1 );
        break;
        case Qt::Key_A:
            camera.eye.setX( camera.eye.x() + 1 );
        break;

        case Qt::Key_S:
            camera.eye.setY( camera.eye.y() - 1 );
        break;
        case Qt::Key_W:
            camera.eye.setY( camera.eye.y() + 1 );
        break;

        case Qt::Key_E:
            camera.eye.setZ( camera.eye.z() - 1 );
        break;
        case Qt::Key_D:
            camera.eye.setZ( camera.eye.z() + 1 );
        break;
    }

    if( player1 != NULL ) player1->keyPressEvent(event);
    if( player2 != NULL ) player2->keyPressEvent(event);
}

void OpenGLWidget::keyReleaseEvent( QKeyEvent * event ){
    if( player1 != NULL ) player1->keyReleaseEvent(event);
    if( player2 != NULL ) player2->keyReleaseEvent(event);
}

void OpenGLWidget :: mouseMoveEvent ( QMouseEvent * event ){
}
void OpenGLWidget :: mousePressEvent ( QMouseEvent * event ){
    /*
    if ( event -> button () & Qt :: LeftButton )
    trackBall . mousePress ( event-> localPos () ) ;
    */
}
void OpenGLWidget :: mouseReleaseEvent ( QMouseEvent * event ){
    /*
    if ( event -> button () == Qt :: LeftButton )
    trackBall . mouseRelease ( event-> localPos () ) ;
    */
}
//----------------------------------------------------------------------------------------------
void OpenGLWidget::gameLogic(float deltaTime){

    if( player1 != NULL ) player1->update(deltaTime);
    if( player2 != NULL ) player2->update(deltaTime);
}

void OpenGLWidget::loadLevel(){
    char cenario[LINES][COLUMNS] = {
        {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
        {'X','H','.','.','B','B','B','B','B','B','B','.','.','.','X'},
        {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
        {'X','.','B','B','B','B','B','B','B','B','B','B','B','B','X'},
        {'X','B','X','B','X','B','X','.','X','B','X','B','X','B','X'},
        {'X','B','B','B','B','B','B','.','B','B','.','B','B','B','X'},
        {'X','B','X','B','X','.','.','.','.','.','X','B','X','B','X'},
        {'X','B','B','B','B','B','B','.','B','B','.','B','.','B','X'},
        {'X','B','X','B','X','B','X','.','X','B','X','B','X','.','X'},
        {'X','.','B','B','B','B','B','B','.','B','.','B','B','.','X'},
        {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
        {'X','.','.','.','B','B','.','B','B','B','B','.','.','.','X'},
        {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
    };
/*
    Entity * bomberman = new Entity("bomberman", "bomberman.mesh.xml");
    bomberman->setPosition( QVector3D(COLUMNS/2, 0, LINES /2 * -1 -1) );
    bomberman->setScale(QVector3D(0.3f, 0.3f, 0.3f) );
    objects.push_back( bomberman );
*/
    light.position = QVector4D(0 , 10.0f , LINES /2 * -1 , 0.0f);


    camera.eye = QVector3D( COLUMNS / 2, 10.0f, -1.0f );
    camera.at  = QVector3D( COLUMNS / 2, 0.0f, LINES / 2 * - 1 + 0.5f);

    qDebug() << light.position;

    Entity * field = new Entity( "field", "scenario_green.mesh.xml" );
    field->translate( QVector3D(6, -1.0f ,0) );
    field->setScale( QVector3D(10.0f, 10.0f, 10.0f) );
    field->rotate( -90, QVector3D(1, 0, 0) );
    objects.push_back(field);

    for( int i = 0; i < LINES; i++ ){
        for( int j = 0; j < COLUMNS; j++){
            Entity * ent = NULL;

            switch( cenario[i][j] ){
            case 'X':
                ent = new Entity( "hard_block", "block_hard.mesh.xml" );

                ent->setScale( QVector3D( 0.5f, 0.5f, 0.5f ) );
                break;
            case 'B':
                ent = new Entity( "normal_block", "block_normal.mesh.xml" );

                ent->setScale( QVector3D( 0.5f, 0.5f, 0.5f ) );

                break;
            case 'H':
                ent = new Entity("bomberman", "bomberman.mesh.xml");
                ent->setScale(QVector3D(0.3f, 0.3f, 0.3f) );

                if(player1 == NULL){
                    player1 = new GamePlayer( ent );
                }

                if(player2 == NULL){
                    player2 = new GamePlayer( ent );
                }

                break;
            }

            if( ent != NULL ){
                ent->setPosition( QVector3D( j, 0, i - LINES ) );
                objects.push_back( ent );
            }
        }
       // if( i == 2 )break;
    }
    qDebug() << " Level Load End...";


    /*
    Entity * bomberman = new Entity("bomberman", "bomberman.mesh.xml");
    bomberman->setPosition( QVector3D(6, 0, 0) );
    bomberman->setScale(QVector3D(0.3f, 0.3f, 0.3f) );
    objects.push_back( bomberman );
    */

    /*
    Entity * bomberman2 = new Entity("bomberman2", "bomberman.mesh.xml");
    bomberman2->setPosition( QVector3D(10, 0, -10) );
    bomberman2->setScale(QVector3D(0.3f, 0.3f, 0.3f) );
    objects.push_back( bomberman2 );
    */

    /*


    Entity * bomb = new Entity("bomb", "bomb.mesh.xml");
    bomb->translate( QVector3D(-2, 0, -5) );
    bomb->rotate(180, QVector3D(0, 1, 0) );
    objects.push_back( bomb );

    qDebug() << bomberman->getModelView();
    qDebug() << bomb->getModelView();
*/
}
