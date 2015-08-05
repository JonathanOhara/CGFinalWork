# include "camera.h"
Camera :: Camera (){
    eye = QVector3D (7.5 , 9.0 , 2.0f) ;
    at = QVector3D (7.5f , 0.0f , -5.0f) ;
    up = QVector3D (0.0 , 1.0 , 0.0) ;
}
