// compile with the following command:
//     clear; g++ -o simulation main.cpp ClothSimulator.cpp Node.cpp Camera.cpp Constraint.cpp StructuralConstraint.cpp ShearConstraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp Floor.cpp Scene.cpp BatmanScene.cpp Keyboard.cpp DrawingSettings.cpp -lglut -lGLU -lGL; ./simulation

#include "ClothSimulator.h"
#include "Keyboard.h"

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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(800, 800);

    glutCreateWindow("Cloth Simulator");

    // create the scene
    ClothSimulator::getInstance()->createScene();

    glutDisplayFunc(display);
    glutIdleFunc(applyChanges);
    glutReshapeFunc(reshape);

    // disable keyboard repeat, because we will use variables for continuous animation
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(normalKeyboardInput);
    glutKeyboardUpFunc(normalKeyboardRelease);
    glutSpecialFunc(specialKeyboardInput);

    glutMainLoop();
    return 0;
}

// used for idle callback.
// Will process all changes that occur, such as keyboard commands, new forces, ...
void applyChanges()
{
    ClothSimulator::getInstance()->simulate();

    // redraw the screen
    glutPostRedisplay();
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

    ClothSimulator* clothSimulator = ClothSimulator::getInstance();
    Camera* camera = clothSimulator->getScene()->getCamera();
    Vector3 cameraPosition = camera->getPosition();
    Vector3 cameraViewDirection = camera->getViewDirection();
    Vector3 cameraUpDirection = camera->getUpDirection();

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
    Keyboard::getInstance()->handleNormalKeyboardInput(key, x, y);
}

void normalKeyboardRelease(unsigned char key, int x, int y)
{
    Keyboard::getInstance()->handleNormalKeyboardRelease(key, x, y);
}

void specialKeyboardInput(int key, int x, int y)
{
    Keyboard::getInstance()->handleSpecialKeyboardInput(key, x, y);
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    // reset projection matrix
    glLoadIdentity();

    // set window size to new values
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    float nearPlane = ClothSimulator::getInstance()->getScene()->getNearPlane();
    float farPlane = ClothSimulator::getInstance()->getScene()->getFarPlane();

    float aspectRatio = (1.0 * w) / h;
    gluPerspective(60.0, aspectRatio, nearPlane, farPlane);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}