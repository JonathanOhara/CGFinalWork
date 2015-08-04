#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]){

    QSurfaceFormat format ;
    format . setVersion (4, 1);
    format . setDepthBufferSize (24) ;
    // Multisampling for antialiasing effect
    format . setSamples (4);
    // CoreProfile or CompatibilityProfile can be used . If Core
    //Profile is chosen , pay attention for more recent OpenGL
    //version you will need to declare and use at least one Vertex
    //Array Object (VAO)
    // For MacOS , CoreProfile has being mandatory .
    format . setProfile ( QSurfaceFormat :: CompatibilityProfile );
    QSurfaceFormat :: setDefaultFormat ( format );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
