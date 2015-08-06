#include "entity.h"
#include <math.h>

Entity::Entity(string name, string fileName){
    this->name = name;
    this->fileName = fileName;

    vertices = NULL;
    indices = NULL;
    normals = NULL;

    vboVertices = NULL;
    vboIndices = NULL;
    vboNormals = NULL;

    shaderProgram = NULL ;
    vertexShader = NULL ;
    fragmentShader = NULL ;

    textureCoordinates = NULL;
    tangents = NULL;

    readMeshXml();
    readMaterial();
    generateTangents();

    createVBOs();

    transformation.setToIdentity() ;
}

Entity::~Entity(){

}

void Entity::readMeshXml(){

    QString path;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root, *geometry, *subMeshes, *skeletonLink, *boneAssignments, *subMesh, *faces, *vertexBuffer, *child, *position, *normal, *texCoord;
    int i;

    path = QDir::currentPath();
    path.remove( path.lastIndexOf("/"), path.length() - path.lastIndexOf("/") );
    path += "/CGFinalWork/media/mesh/" + QString(fileName.c_str());

    qDebug() << "Loading mesh: " <<path;
    std::ifstream if2( path.toStdString().c_str(), std::ifstream::in);

    std::vector<char> buffer((std::istreambuf_iterator<char>(if2)), std::istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    root = doc.first_node("mesh");
    geometry = root->first_node("sharedgeometry");
    subMeshes = root->first_node("submeshes");
    skeletonLink = root->first_node("skeletonlink");
    boneAssignments = root->first_node("boneassignments");
    vertexBuffer = geometry->first_node("vertexbuffer");

    vertexCount = atoi( geometry->first_attribute("vertexcount")->value() );
    qDebug() << "Vertex Count: " << vertexCount;

    vertices = new QVector4D[ vertexCount ];
    normals = new QVector3D[ vertexCount ];
    textureCoordinates = new QVector2D[ vertexCount ];

    qDebug() << "Loading vertices...";
    i = 0;
    for ( child = vertexBuffer->first_node("vertex"); child; child = child->next_sibling()){

        position = child->first_node("position");
        normal = child->first_node("normal");
        texCoord = child->first_node("texcoord");

        vertices[i] = QVector4D(
            atof( position->first_attribute("x")->value() ),
            atof( position->first_attribute("y")->value() ),
            atof( position->first_attribute("z")->value() ),
            1.0f
        );

        normals[i] = QVector3D(
            atof( normal->first_attribute("x")->value() ),
            atof( normal->first_attribute("y")->value() ),
            atof( normal->first_attribute("z")->value() )
        );

        textureCoordinates[i] = QVector2D(
            atof( texCoord->first_attribute("u")->value() ),
            atof( texCoord->first_attribute("v")->value() )
        );

        i++;
    }

    qDebug() << "Done.";

    subMesh = subMeshes->first_node("submesh");

    materialName = subMesh->first_attribute("material")->value();
    qDebug() << "Material Name: " << materialName.c_str();

    faces = subMesh->first_node("faces");

    facesCount = atoi( faces->first_attribute("count")->value() );

    indices = new unsigned int[ facesCount * 3 ];

    qDebug() << "Loading faces...";
    i = 0;
    for ( child = faces->first_node("face"); child; child = child->next_sibling()){
        indices [ i * 3 ] = atoi( child->first_attribute("v1")->value() );
        indices [ i * 3 + 1] = atoi( child->first_attribute("v2")->value() );
        indices [ i * 3 + 2] = atoi( child->first_attribute("v3")->value() );

        i++;
    }
    qDebug() << "Done.";

    if( skeletonLink != NULL ){
        skeletonFileName = skeletonLink->first_attribute("name")->value();
        qDebug() << "Skeleton File Name: " << skeletonFileName.c_str();

        qDebug() << "Loading bone assigments...";
        i = 0;
        for ( child = faces->first_node("vertexboneassignment"); child; child = child->next_sibling()){

            int boneIndex = atoi( child->first_attribute("boneindex")->value() );
            int vertexIndex = atoi( child->first_attribute("vertexindex")->value() );
            float weight = atof( child->first_attribute("weight")->value() );

            i++;
        }
        qDebug() << "Done.";
    }

}

void Entity::readMaterial(){
    QString path;

    material = new Material();

    path = QDir::currentPath();
    path.remove( path.lastIndexOf("/"), path.length() - path.lastIndexOf("/") );
    path += "/CGFinalWork/media/material/" + QString(materialName.c_str()) + ".material";

    qDebug() << path;
    ifstream ifs( path.toStdString().c_str(), std::ifstream::in);

    sh::ConfigLoader * loader = new sh::ConfigLoader("");
    loader->parseScript(ifs);

    sh::ConfigNode *rootNode, *techniqueNode, *passNode, *textureUnitNode, *ambient, *diffuse, *specular, *emissive;

    rootNode = loader->getConfigScript("material", materialName);
    techniqueNode = rootNode->findChild("technique");
    passNode = techniqueNode->findChild("pass");
    textureUnitNode = passNode->findChild("texture_unit");

    ambient = passNode->findChild("ambient");
    diffuse = passNode->findChild("diffuse");
    specular = passNode->findChild("specular");
    emissive = passNode->findChild("emissive");

    material->ambient = QVector4D(
        atof( ambient->getValue(0).c_str() ),
        atof( ambient->getValue(1).c_str() ),
        atof( ambient->getValue(2).c_str() ),
        atof( ambient->getValue(3).c_str() )
    );

    material->diffuse = QVector4D(
        atof( diffuse->getValue(0).c_str() ),
        atof( diffuse->getValue(1).c_str() ),
        atof( diffuse->getValue(2).c_str() ),
        atof( diffuse->getValue(3).c_str() )
    );

    material->specular = QVector4D(
        atof( specular->getValue(0).c_str() ),
        atof( specular->getValue(1).c_str() ),
        atof( specular->getValue(2).c_str() ),
        atof( specular->getValue(3).c_str() )
    );

    material->textureName = textureUnitNode->findChild("texture")->getValue().c_str();

    path = QDir::currentPath();
    path.remove( path.lastIndexOf("/"), path.length() - path.lastIndexOf("/") );
    path += "/CGFinalWork/media/images/" + material->textureName;

    qDebug() << "Loading image: " << path;

    QImage image;
    image . load ( path ) ;

    material->texture = new QOpenGLTexture ( image ) ;
    material->texture->setMinificationFilter( QOpenGLTexture::LinearMipMapLinear ) ;
    material->texture->setMagnificationFilter( QOpenGLTexture::Linear ) ;
    material->texture->setWrapMode( QOpenGLTexture::Repeat ) ;

    int currentShader = 0;

    QString vertexShaderFile [] = {
         ":/shaders/vert/phong.vert"
     };
     QString fragmentShaderFile [] = {
         ":/shaders/frag/phong.frag"
     };
     vertexShader = new QOpenGLShader ( QOpenGLShader :: Vertex ) ;

     qDebug() << "Loading " << vertexShaderFile [currentShader ] << " and "
              << fragmentShaderFile [currentShader ];

     if (! vertexShader -> compileSourceFile ( vertexShaderFile [currentShader ]) )
         qWarning () << vertexShader -> log () ;

     fragmentShader = new QOpenGLShader ( QOpenGLShader :: Fragment ) ;

     if (! fragmentShader -> compileSourceFile ( fragmentShaderFile [currentShader ]) )
         qWarning () << fragmentShader -> log () ;

     shaderProgram = new QOpenGLShaderProgram ;
     shaderProgram -> addShader ( vertexShader ) ;
     shaderProgram -> addShader ( fragmentShader ) ;

     if (! shaderProgram -> link () )
         qWarning () << shaderProgram -> log () << endl ;

    delete loader;
}

void Entity::generateTangents () {
    if ( tangents ) delete [] tangents ;
    tangents = new QVector4D [ vertexCount ];
    QVector3D * bitangents = new QVector3D [ vertexCount ];
    for ( unsigned int i = 0; i < facesCount ; i ++) {
        unsigned int i1 = indices [ i * 3 ];
        unsigned int i2 = indices [ i * 3 + 1];
        unsigned int i3 = indices [ i * 3 + 2];
        QVector3D E = vertices [ i1 ]. toVector3D () ;
        QVector3D F = vertices [ i2 ]. toVector3D () ;
        QVector3D G = vertices [ i3 ]. toVector3D () ;
        QVector2D stE = textureCoordinates [ i1 ];
        QVector2D stF = textureCoordinates [ i2 ];
        QVector2D stG = textureCoordinates [ i3 ];
        QVector3D P = F - E ;
        QVector3D Q = G - E ;
        QVector2D st1 = stF - stE ;
        QVector2D st2 = stG - stE ;

        QMatrix2x2 M ;
        M (0 ,0) = st2 . y () ; M (0 ,1) = - st1 . y () ;
        M (1 ,0) = - st2 . x () ; M (1 ,1) = st1 . x () ;
        M *= (1.0 / ( st1 . x () * st2 . y () - st2 . x () * st1 . y () ) ) ;

        QVector4D T = QVector4D ( M (0 ,0) * P . x () + M (0 ,1) * Q . x () ,
            M (0 ,0) * P . y () + M (0 ,1) * Q . y () ,
            M (0 ,0) * P . z () + M (0 ,1) * Q . z () , 0.0);

        QVector3D B = QVector3D ( M (1 ,0) * P . x () + M (1 ,1) * Q . x () ,
        M (1 ,0) * P . y () + M (1 ,1) * Q . y () ,
        M (1 ,0) * P . z () + M (1 ,1) * Q . z () ) ;
        tangents [ i1 ] += T ;
        tangents [ i2 ] += T ;
        tangents [ i3 ] += T ;
        bitangents [ i1 ] += B ;
        bitangents [ i2 ] += B ;
        bitangents [ i3 ] += B ;
    }

    for ( unsigned int i = 0; i < vertexCount ; i ++) {
        const QVector3D & n = normals [ i ];
        const QVector4D & t = tangents [ i ];
        tangents [ i ] = ( t - n *
        QVector3D :: dotProduct (n , t . toVector3D () ) ) .
        normalized () ;
        QVector3D b = QVector3D :: crossProduct (n , t . toVector3D () ) ;
        double hand = QVector3D :: dotProduct (b , bitangents [ i ]) ;
        tangents [ i ]. setW (( hand < 0.0) ? -1.0 : 1.0) ;
    }
    delete [] bitangents ;
}

void Entity::createVBOs(){
    vboVertices = new QOpenGLBuffer ( QOpenGLBuffer :: VertexBuffer ) ;
    vboVertices -> create () ;
    vboVertices -> bind () ;
    vboVertices -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboVertices -> allocate ( vertices , vertexCount * sizeof ( QVector4D ) ) ;
    delete [] vertices ;
    vertices = NULL ;



    vboIndices = new QOpenGLBuffer ( QOpenGLBuffer :: IndexBuffer ) ;
    vboIndices -> create () ;
    vboIndices -> bind () ;
    vboIndices -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboIndices -> allocate ( indices , facesCount * 3 * sizeof ( unsigned int ) ) ;
    delete [] indices ;
    indices = NULL ;



    vbocoordText = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbocoordText->create();
    vbocoordText->bind();
    vbocoordText->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbocoordText->allocate(textureCoordinates, vertexCount * sizeof(QVector2D));
    delete[] textureCoordinates;
    textureCoordinates = NULL;



    vboNormals = new QOpenGLBuffer ( QOpenGLBuffer :: VertexBuffer ) ;
    vboNormals -> create () ;
    vboNormals -> bind () ;
    vboNormals -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboNormals -> allocate ( normals , vertexCount * sizeof ( QVector3D ) ) ;
    delete [] normals ;
    normals = NULL ;



    vboTangents = new QOpenGLBuffer ( QOpenGLBuffer :: VertexBuffer ) ;
    vboTangents -> create () ;
    vboTangents -> bind () ;
    vboTangents -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboTangents -> allocate ( tangents , vertexCount * sizeof ( QVector4D ) ) ;
    delete [] tangents ;
    tangents = NULL ;
}

void Entity::destroyVBOs(){
    if ( vboVertices ) {
         vboVertices -> release () ;
         vboVertices -> destroy ();
         delete vboVertices ;
         vboVertices = NULL ;
     }

     if ( vboIndices ) {
         vboIndices -> release ();
         vboIndices -> destroy ();
         delete vboIndices ;
         vboIndices = NULL ;
     }

     if( vbocoordText ){
         vbocoordText->release();
         vbocoordText->destroy();
         delete vbocoordText;
         vbocoordText = NULL;
     }

     if( vboNormals ){
         vboNormals->release();
         vboNormals->destroy();
         delete vboNormals;
         vboNormals = NULL;
     }

     if( vboTangents ){
         vboTangents->release();
         vboTangents->destroy();
         delete vboTangents;
         vboTangents = NULL;
     }
}

QVector3D Entity::getPosition(){
    return transformation.column(3).toVector3D();
}

void Entity::setPosition( QVector3D position ){
   transformation.setColumn( 3, QVector4D( position, 1 ) );
}

void Entity::roundX(){
    transformation.data()[12] = round( transformation.data()[12] );
}

void Entity::roundZ(){
    transformation.data()[14] = round( transformation.data()[14] );
}

void Entity::setScale( QVector3D scale ){
   transformation.scale( scale );
}

void Entity::clearRotation(){
    //qDebug() << "antes " << transformation;
    transformation.data()[0] = abs(transformation.data()[0] + transformation.data()[1] + transformation.data()[2]  + transformation.data()[3]);
    transformation.data()[5] = abs(transformation.data()[4] + transformation.data()[5] + transformation.data()[6]  + transformation.data()[7]);
    transformation.data()[10] = abs(transformation.data()[8] + transformation.data()[9] + transformation.data()[10] + transformation.data()[11]);


    transformation.data()[1] = 0;transformation.data()[2] = 0;transformation.data()[3] = 0;transformation.data()[4] = 0;
    transformation.data()[6] = 0;transformation.data()[7] = 0;transformation.data()[8] = 0;transformation.data()[9] = 0;
    transformation.data()[11] = 0;

    //qDebug() << "depois " << transformation;
}

void Entity::rotate( float angle, QVector3D vector ){
   transformation.rotate( angle, vector );
}

void Entity::translate( QVector3D translate ){
   transformation.translate( translate );
}

/******************** GETTERS AND SETTERS ********************/
string Entity::getName(){
    return name;
}
string Entity::getfileName(){
    return fileName;
}
QOpenGLBuffer * Entity::getVboVertices(){
    return vboVertices;
}
QOpenGLBuffer * Entity::getVboIndices(){
    return vboIndices;
}
QOpenGLBuffer * Entity::getVbocoordText(){
    return vbocoordText;
}
QOpenGLBuffer * Entity::getVboNormals(){
    return vboNormals;
}
QOpenGLBuffer * Entity::getVboTangents(){
    return vboTangents;
}
QOpenGLShader * Entity::getVertexShader(){
    return vertexShader;
}
QOpenGLShader * Entity::getFragmentShader(){
    return fragmentShader;
}
QOpenGLShaderProgram * Entity::getShaderProgram(){
    return shaderProgram;
}
QMatrix4x4 Entity::getTransformation(){
    return transformation;
}
Material * Entity::getMaterial(){
    return material;
}
string Entity::getMaterialName(){
    return materialName;
}
int Entity::getVertexCount(){
    return vertexCount;
}
int Entity::getFacesCount(){
    return facesCount;
}
