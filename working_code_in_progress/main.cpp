// compile with the following command:
//     g++ -o simulation main.cpp ClothSimulator.cpp Node.cpp Camera.cpp Constraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp Floor.cpp Scene.cpp BatmanScene.cpp -lglut -lGLU -lGL;

#include "ClothSimulator.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void display();
void reshape(int w, int h);
void normalKeyboardInput(unsigned char key, int x, int y);
void normalKeyboardRelease(unsigned char key, int x, int y);
void specialKeyboardInput(int key, int x, int y);
void applyChanges();

ClothSimulator* clothSimulator = 0;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(1520, 800);

    clothSimulator = ClothSimulator::getInstance();

    glutCreateWindow("Cloth Simulator");

    // create the scene
    clothSimulator->createScene();

    glutDisplayFunc(display);
    // glutIdleFunc(applyChanges);
    glutReshapeFunc(reshape);

    // disable keyboard repeat, because we will use variables for continuous animation
    glutIgnoreKeyRepeat(1);

    // glutKeyboardFunc(normalKeyboardInput);
    // glutKeyboardUpFunc(normalKeyboardRelease);
    // glutSpecialFunc(specialKeyboardInput);

    glutMainLoop();
    return 0;
}

// used for idle callback.
// Will process all changes that occur, such as keyboard commands, new forces, ...
void applyChanges()
{

    // clothSimulator->simulate();

    // // redraw the screen
    // glutPostRedisplay();
}

// The display function only takes care of drawing.
// No modifications to any forces, keyboard commands, ... are processed here.
void display()
{
    // clear color buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector3 cameraPosition = clothSimulator->getCameraPosition();
    Vector3 cameraViewDirection = clothSimulator->getCameraViewDirection();
    Vector3 cameraUpDirection = clothSimulator->getCameraUpDirection();

    // all perspectives are already calculated and stored, so just position
    // the camera where it is supposed to be
    gluLookAt(cameraPosition.x,
              cameraPosition.y,
              cameraPosition.z,
              cameraPosition.x + cameraViewDirection.x,
              cameraPosition.y + cameraViewDirection.y,
              cameraPosition.z + cameraViewDirection.z,
              cameraUpDirection.x,
              cameraUpDirection.y,
              cameraUpDirection.z);

    clothSimulator->draw();

    // swap buffers needed for double buffering
    glutSwapBuffers();
}

void normalKeyboardInput(unsigned char key, int x, int y)
{
    // clothSimulator->handleNormalKeyboardInput(key, x, y);
}

void normalKeyboardRelease(unsigned char key, int x, int y)
{
    // clothSimulator->handleNormalKeyboardRelease(key, x, y);
}

void specialKeyboardInput(int key, int x, int y)
{
    // clothSimulator->handleSpecialKeyboardInput(key, x, y);
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    // reset projection matrix
    glLoadIdentity();

    // set window size to new values
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // set a big clipping plane for now (no display errors)
    float aspectRatio = (1.0 * w) / h;
    // gluPerspective(60.0, aspectRatio, clothSimulator->nearPlane, clothSimulator->farPlane);
    gluPerspective(60.0, aspectRatio, 1.0, 1000.0);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}