#include "openglwidget.h"

OpenGLWidget::OpenGLWidget ( QWidget * parent ):  QOpenGLWidget ( parent ) {

}

OpenGLWidget::~OpenGLWidget (){
    /*
    destroyVBOs () ;
    destroyShaders () ;
    */
}

void OpenGLWidget :: initializeGL () {
    initializeOpenGLFunctions () ;
    std :: cerr << " Version " << glGetString ( GL_VERSION ) <<"\n";
    std :: cerr << " GLSL " << glGetString ( GL_SHADING_LANGUAGE_VERSION )<<"\n";
    glEnable ( GL_DEPTH_TEST );

    connect (&timer , SIGNAL ( timeout () ) , this , SLOT ( animate () ) ) ;

    loadLevel();

    timer.start (0) ;
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
    //    qDebug() << "paintGL 3";

    modelView.setToIdentity () ;
    modelView.lookAt ( camera.eye , camera.at , camera.up ) ;

    QMatrix4x4 modelViewMatrix;

    while( object != objects.end() ){
//        qDebug() << "paintGL 4";
        obj = (*object);

//        qDebug() << "paintGL 5";

        obj->getShaderProgram()->bind();
//        qDebug() << "paintGL 6";

        ambientProduct = light.ambient * obj->getMaterial()->ambient ;
        diffuseProduct = light.diffuse * obj->getMaterial()->diffuse ;
        specularProduct = light.specular * obj->getMaterial()->specular ;

//        qDebug() << "paintGL 7";

        modelViewMatrix = modelView * obj->getTransformation();

        obj->getShaderProgram()->setUniformValue("lightPosition", light.position ) ;
        obj->getShaderProgram()->setUniformValue("ambientProduct", ambientProduct ) ;
        obj->getShaderProgram()->setUniformValue("diffuseProduct", diffuseProduct ) ;
        obj->getShaderProgram()->setUniformValue("specularProduct", specularProduct ) ;
        obj->getShaderProgram()->setUniformValue("shininess", static_cast < GLfloat >( material.shininess ) ) ;

        obj->getShaderProgram()->setUniformValue("modelView", modelViewMatrix ) ;
        obj->getShaderProgram()->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix() ) ;
        obj->getShaderProgram()->setUniformValue("projectionMatrix", projectionMatrix );

//        qDebug() << "paintGL 8";

        obj->getVboVertices()-> bind () ;
        obj->getShaderProgram() -> enableAttributeArray ("vPosition") ;
        obj->getShaderProgram() -> setAttributeBuffer ("vPosition", GL_FLOAT , 0 , 4 , 0) ;

//        qDebug() << "paintGL 9";

        obj->getVboNormals()->bind();
        obj->getShaderProgram() -> enableAttributeArray ("vNormal") ;
        obj->getShaderProgram() -> setAttributeBuffer ("vNormal", GL_FLOAT , 0 , 3 , 0) ;

//        qDebug() << "paintGL 95";

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

//        qDebug() << "paintGL 20";

        glDrawElements ( GL_TRIANGLES , obj->getFacesCount() * 3 , GL_UNSIGNED_INT , 0) ;
//        qDebug() << "paintGL 21";

        obj->getShaderProgram()->release();
        obj->getVboVertices()->release();
        obj->getVboNormals()->release();
        obj->getVboIndices()->release();
        obj->getVbocoordText()->release();
        obj->getMaterial()->texture->release(0);

//        qDebug() << "paintGL 22";

        object++;
    }
}


void OpenGLWidget :: animate (){
    update () ;
}

//----------------------------------------------------------------------------------------------
void OpenGLWidget::keyPressEvent( QKeyEvent * event ){
    qDebug() << "press";
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
    qDebug() << camera.eye;
}

void OpenGLWidget :: mouseMoveEvent ( QMouseEvent * event ){
    qDebug() << "a";
}
void OpenGLWidget :: mousePressEvent ( QMouseEvent * event ){
    qDebug() << "b";
    /*
    if ( event -> button () & Qt :: LeftButton )
    trackBall . mousePress ( event-> localPos () ) ;
    */
}
void OpenGLWidget :: mouseReleaseEvent ( QMouseEvent * event ){
    qDebug() << "c";
    /*
    if ( event -> button () == Qt :: LeftButton )
    trackBall . mouseRelease ( event-> localPos () ) ;
    */
}
//----------------------------------------------------------------------------------------------
void OpenGLWidget::loadLevel(){
    char cenario[13][15] = {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X','H','.','.','B','B','B','B','B','B','B','.','.','.','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','B','B','B','B','B','B','B','B','B','B','B','B','X'},
    {'X','B','X','B','X','B','X','.','X','B','X','B','X','B','X'},
    {'X','B','B','B','B','B','B','.','B','B','.','B','B','B','X'},
    {'X','B','X','B','X','B','X','.','X','.','X','B','X','B','X'},
    {'X','B','B','B','B','B','B','.','B','B','.','B','.','B','X'},
    {'X','B','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','B','B','B','B','B','B','.','B','.','B','B','.','X'},
    {'X','.','X','B','X','B','X','B','X','B','X','B','X','.','X'},
    {'X','.','.','.','B','B','.','B','B','B','B','.','.','.','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'},
    };

    light.position = QVector4D(6.0f , 120.0f , 80.0f , 0.0f);

    Entity * field = new Entity( "field", "scenario_green.mesh.xml" );
    field->translate( QVector3D(6, -1.0f ,0) );
    field->setScale( QVector3D(10.0f, 10.0f, 10.0f) );
    field->rotate( -90, QVector3D(1, 0, 0) );


    objects.push_back(field);


    Entity * bomberman = new Entity("bomberman", "bomberman.mesh.xml");
    bomberman->setPosition( QVector3D(6, 0, 0) );
    bomberman->setScale(QVector3D(0.3f, 0.3f, 0.3f) );
    objects.push_back( bomberman );

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
