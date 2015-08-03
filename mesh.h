#ifndef MESH_H
#define MESH_H

#include <Qt>
#include <QtCore>
#include <QtOpenGL>
#include <QVector4D>

#include <string>
#include <rapidxml.hpp>
#include <fstream>

#include "ConfigLoader.hpp"
#include "Material.h"

using namespace std;
using namespace Qt;

class Mesh{
public:
    Mesh(string name, string fileName);
    ~Mesh();

    QOpenGLBuffer * vboVertices ;
    QOpenGLBuffer * vboIndices ;
    QOpenGLBuffer * vbocoordText ;
    QOpenGLBuffer * vboNormals ;

    //Shaders
    QOpenGLShader * vertexShader ;
    QOpenGLShader * fragmentShader ;
    QOpenGLShaderProgram * shaderProgram ;

    Material * material;

    string materialName;
    int vertexCount;
    int facesCount;
private:
    void readMeshXml();
    void readMaterial();
    void createVBOs();
    void destroyVBOs();

    string name;
    string fileName;

    string skeletonFileName;

    QVector4D *vertices;
    unsigned int * indices;
    QVector2D *textureCoordinates;
    QVector3D *normals;




};

#endif // MESH_H
