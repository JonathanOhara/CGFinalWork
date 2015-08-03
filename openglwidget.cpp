#include "openglwidget.h"

OpenGLWidget::OpenGLWidget ( QWidget * parent ):  QOpenGLWidget ( parent ) {
    /*
    vertices = NULL;
    colors = NULL;
    indices = NULL;
    normals = NULL;

    vboVertices = NULL;
    vboColors = NULL;
    vboIndices = NULL;
    vboNormals = NULL;

    shaderProgram = NULL ;
    vertexShader = NULL ;
    fragmentShader = NULL ;

    texCoords = NULL;
    */
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

    timer.start (0) ;


    objects.push_back( new Mesh("bomberman", "bomberman.mesh.xml") );


}

void OpenGLWidget :: resizeGL (int w , int h ) {
    glViewport (0 , 0 , w , h ) ;

    projectionMatrix . setToIdentity () ;
    projectionMatrix . perspective (60.0 , static_cast < qreal >( w ) / static_cast < qreal >( h ) , 0.1 , 20.0) ;

    update();
}

void OpenGLWidget :: paintGL () {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

//    qDebug() << "paintGL 1";

    if( objects.size() == 0 ) return;

    std::list<Mesh*>::iterator object = objects.begin();

//    qDebug() << "paintGL 2";
    Mesh * obj;
    //    qDebug() << "paintGL 3";

    while( object != objects.end() ){
//        qDebug() << "paintGL 4";
        obj = (*object);

//        qDebug() << "paintGL 5";
        modelView.setToIdentity() ;
        modelView.lookAt( camera.eye , camera.at , camera.up );
        modelView.scale( 1 , 1 , 1 ) ;
        modelView.translate( QVector3D(0, 0, -4) );
        modelView.rotate(180, 0, 1, 0);


        obj->shaderProgram->bind();
//        qDebug() << "paintGL 6";

        ambientProduct = light.ambient * obj->material->ambient ;
        diffuseProduct = light.diffuse * obj->material->diffuse ;
        specularProduct = light.specular * obj->material->specular ;

//        qDebug() << "paintGL 7";

        obj->shaderProgram -> setUniformValue ("lightPosition", light.position ) ;
        obj->shaderProgram -> setUniformValue ("ambientProduct", ambientProduct ) ;
        obj->shaderProgram -> setUniformValue ("diffuseProduct", diffuseProduct ) ;
        obj->shaderProgram -> setUniformValue ("specularProduct", specularProduct ) ;
        obj->shaderProgram -> setUniformValue ("shininess", static_cast < GLfloat >( material.shininess ) ) ;

        obj->shaderProgram -> setUniformValue ("modelView", modelView ) ;
        obj->shaderProgram -> setUniformValue ("normalMatrix", modelView.normalMatrix () ) ;
        obj->shaderProgram -> setUniformValue ("projectionMatrix", projectionMatrix );

//        qDebug() << "paintGL 8";

        obj->vboVertices-> bind () ;
        obj->shaderProgram -> enableAttributeArray ("vPosition") ;
        obj->shaderProgram -> setAttributeBuffer ("vPosition", GL_FLOAT , 0 , 4 , 0) ;

//        qDebug() << "paintGL 9";

        obj->vboNormals->bind();
        obj->shaderProgram -> enableAttributeArray ("vNormal") ;
        obj->shaderProgram -> setAttributeBuffer ("vNormal", GL_FLOAT , 0 , 3 , 0) ;

//        qDebug() << "paintGL 10";

        obj->vboIndices -> bind () ;

//        qDebug() << "paintGL 11";

        obj->vbocoordText -> bind ();
        obj->shaderProgram -> enableAttributeArray ("vcoordText") ;
        obj->shaderProgram -> setAttributeBuffer ("vcoordText", GL_FLOAT , 0 , 2 , 0) ;

//        qDebug() << "paintGL 12";

        obj->material->texture -> bind (0) ;
        obj->shaderProgram -> setUniformValue ("colorTexture", 0) ;

//        qDebug() << "paintGL 20";

        glDrawElements ( GL_TRIANGLES , obj->facesCount * 3 , GL_UNSIGNED_INT , 0) ;
//        qDebug() << "paintGL 21";

        obj->shaderProgram->release();
        obj->vboVertices->release();
        obj->vboNormals->release();
        obj->vboIndices->release();
        obj->vbocoordText->release();
        obj->material->texture->release(0);

//        qDebug() << "paintGL 22";

        object++;
    }
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
    }
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
