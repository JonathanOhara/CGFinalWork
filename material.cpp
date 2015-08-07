#include "material.h"
Material::Material (){
    textureName = "" ;
    map1Name = "" ;

    ambient = QVector4D (0.5 , 0.5 , 0.5 , 1.0) ;
    diffuse = QVector4D (0.2 , 0.0 , 0.2 , 1.0) ;
    specular = QVector4D (1 , 1 ,.5 , 1.0) ;
    shininess = 200.0;

    texture = NULL;
    map1 = NULL;
}
