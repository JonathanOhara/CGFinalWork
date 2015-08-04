# include "camera.h"
Camera :: Camera (){
    eye = QVector3D (6.0 , 11.0 , 5.5f) ;
    at = QVector3D (6.0f , 0.0f , -5.0f) ;
    up = QVector3D (0.0 , 1.0 , 0.0) ;
}
