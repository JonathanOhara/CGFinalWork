#include "mesh.h"

Mesh::Mesh(string name, string fileName){
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

    readMeshXml();
    readMaterial();

    createVBOs();
}

Mesh::~Mesh(){

}

void Mesh::readMeshXml(){

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

void Mesh::readMaterial(){
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

    rootNode = loader->getConfigScript("material", "bomberman_material");
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

void Mesh::createVBOs(){
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
}

void Mesh::destroyVBOs(){
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
}
