#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtOpenGL>
#include <QString>

class Material
{
public :
    Material () ;
    QVector4D ambient ;
    QVector4D diffuse ;
    QVector4D specular ;

    QString textureName;
    QString map1Name;
    QOpenGLTexture *texture, *map1 ;

    double shininess ;
};
# endif // MATERIAL_H
