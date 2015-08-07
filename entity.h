#ifndef ENTITY_H
#define ENTITY_H

#include <Qt>
#include <QtCore>
#include <QtOpenGL>
#include <QVector4D>

#include <string>
#include <rapidxml.hpp>
#include <fstream>

#include "ConfigLoader.hpp"
#include "Material.h"
#include "gamePlayer.h"

using namespace std;
using namespace Qt;

class Entity{
public:
    Entity(string name, string fileName);
    ~Entity();

    string getName();
    string getfileName();

    QOpenGLBuffer * getVboVertices();
    QOpenGLBuffer * getVboIndices();
    QOpenGLBuffer * getVbocoordText();
    QOpenGLBuffer * getVboNormals();
    QOpenGLBuffer * getVboTangents();

    //Shaders
    QOpenGLShader * getVertexShader();
    QOpenGLShader * getFragmentShader();
    QOpenGLShaderProgram * getShaderProgram();

    QVector3D getPosition();
    void setPosition( QVector3D position );

    void roundX();
    void roundZ();

    void setScale( QVector3D scale );
    void clearRotation();
    void rotate( float angle, QVector3D vector );
    void translate( QVector3D translate );
    QMatrix4x4 getTransformation();
    void setTransformation(QMatrix4x4 transformation);

    Material * getMaterial();

    string getMaterialName();
    int getVertexCount();
    int getFacesCount();

    bool live;

private:
    void readMeshXml();
    void readMaterial();
    void generateTangents();
    void createVBOs();
    void destroyVBOs();

    string skeletonFileName;

    QVector4D *vertices;
    unsigned int * indices;
    QVector2D *textureCoordinates;
    QVector3D *normals;
    QVector4D * tangents;

    string name;
    string fileName;

    QOpenGLBuffer * vboVertices ;
    QOpenGLBuffer * vboIndices ;
    QOpenGLBuffer * vbocoordText ;
    QOpenGLBuffer * vboNormals ;
    QOpenGLBuffer * vboTangents ;

    //Shaders
    QOpenGLShader * vertexShader ;
    QOpenGLShader * fragmentShader ;
    QOpenGLShaderProgram * shaderProgram ;

    QMatrix4x4 transformation;

    Material *material;

    string materialName;
    int vertexCount;
    int facesCount;

};

#endif // MESH_H
