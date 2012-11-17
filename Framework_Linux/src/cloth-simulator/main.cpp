//=============================================================================
//
//   Exercise code for the lecture
//
//=============================================================================

#include "ClothViewer.h"



int main(int argc, char** argv) {
    glutInit(&argc, argv);

    ClothViewer window("Cloth Viewer", 800, 800);

    if (argc == 2) {
        window.loadMesh(argv[1]);
    } else if (argc == 3) {
        window.loadMesh(argv[1], argv[2]);
    }


    glutMainLoop();
}
