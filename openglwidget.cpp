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
}

void OpenGLWidget :: resizeGL (int w , int h ) {
    glViewport (0 , 0 , w , h ) ;

    projectionMatrix . setToIdentity () ;
    projectionMatrix . perspective (60.0 , static_cast < qreal >( w ) / static_cast < qreal >( h ) , 0.1 , 20.0) ;

    update();
}

void OpenGLWidget :: paintGL () {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
/*
    if (! vboVertices )
        return ;

    modelView . setToIdentity () ;
    modelView . lookAt ( camera . eye , camera . at , camera . up ) ;
    modelView . rotate ( trackBall . getRotation () ) ;
    modelView . scale ( invDiag , invDiag , invDiag ) ;
    modelView . translate ( - midPoint );

    shaderProgram -> bind () ;

    QVector4D ambientProduct = light.ambient * material.ambient ;
    QVector4D diffuseProduct = light.diffuse * material.diffuse ;
    QVector4D specularProduct = light.specular * material.specular ;

    shaderProgram -> setUniformValue ("lightPosition", light.position ) ;
    shaderProgram -> setUniformValue ("ambientProduct", ambientProduct ) ;
    shaderProgram -> setUniformValue ("diffuseProduct", diffuseProduct ) ;
    shaderProgram -> setUniformValue ("specularProduct", specularProduct ) ;
    shaderProgram -> setUniformValue ("shininess", static_cast < GLfloat >( material.shininess ) ) ;

    shaderProgram -> setUniformValue ("light.position", QVector4D(0.5, 0.5, 0.5, 1) ) ;

    shaderProgram -> setUniformValue ("modelView", modelView ) ;
    shaderProgram -> setUniformValue ("normalMatrix", modelView.normalMatrix () ) ;
    shaderProgram -> setUniformValue ("projectionMatrix", projectionMatrix );


    vboVertices -> bind () ;
    shaderProgram -> enableAttributeArray ("vPosition") ;
    shaderProgram -> setAttributeBuffer ("vPosition", GL_FLOAT , 0 , 4 , 0) ;

    vboNormals->bind();
    shaderProgram -> enableAttributeArray ("vNormal") ;
    shaderProgram -> setAttributeBuffer ("vNormal", GL_FLOAT , 0 , 3 , 0) ;

    vboIndices -> bind () ;

    vbocoordText -> bind ();
    shaderProgram -> enableAttributeArray ("vcoordText") ;
    shaderProgram -> setAttributeBuffer ("vcoordText", GL_FLOAT , 0 , 2 , 0) ;

    texture -> bind (0) ;
    shaderProgram -> setUniformValue ("colorTexture", 0) ;

    textureLayer -> bind (1) ;
    shaderProgram -> setUniformValue ("colorTextureLayer", 1) ;


    glDrawElements ( GL_TRIANGLES , numFaces * 3 , GL_UNSIGNED_INT , 0) ;

    texture->release(0);
    textureLayer->release(1);

    vbocoordText->release();
    vboIndices -> release () ;
    vboVertices -> release () ;
    vboVertices->release();
    shaderProgram -> release () ;
    */
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
