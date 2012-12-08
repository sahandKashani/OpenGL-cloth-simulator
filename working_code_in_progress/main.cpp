// compile with the following command:
//     g++ -o simulation main.cpp ClothSimulator.cpp Node.cpp Camera.cpp Constraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp -lglut -lGLU -lGL;

#include "ClothSimulator.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void display();
void reshape(int w, int h);
void normalKeyboard(unsigned char key, int x, int y);
void normalKeyboardRelease(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
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
    clothSimulator->createScene();

    glutDisplayFunc(display);
    glutIdleFunc(applyChanges);
    glutReshapeFunc(reshape);

    // disable keyboard repeat, because we will use variables for continuous animation
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(normalKeyboard);
    glutKeyboardUpFunc(normalKeyboardRelease);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}

// used for idle callback.
// Will process all changes that occur, such as keyboard commands, new forces, ...
void applyChanges()
{
    // find out what commands the keyboard is sending and apply them
    clothSimulator->applyContinuousKeyboardCommands();

    clothSimulator->swingLeftFoot();

    // calculate time difference from last call to applyChanges()
    float duration = clothSimulator->getTimeStep();

    // apply all forces to the cloth
    clothSimulator->cloth->applyForces(duration);

    // satisfy all constraints of the cloth after forces are applied
    clothSimulator->cloth->satisfyConstraints();

    // handle all collisions with the spheres in the scene
    clothSimulator->cloth->handleSphereIntersections();

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

    Vector3 cameraPosition = clothSimulator->camera->getPosition();
    Vector3 cameraViewDirection = clothSimulator->camera->getViewDirection();
    Vector3 cameraUpDirection = clothSimulator->camera->getUpDirection();

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

    // choose wireframe or solid rendering
    clothSimulator->chooseRenderingMethod();

    // draw the world axis
    clothSimulator->drawWorldAxis();

    // draw spheres
    clothSimulator->drawSpheres();

    // draw triangles
    clothSimulator->drawTriangles();

    // draw cloth
    clothSimulator->drawCloth();

    // swap buffers needed for double buffering
    glutSwapBuffers();
}

void normalKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        // ESC control
        case 27:
            // clean up the clothSimulator before exiting
            delete clothSimulator;
            exit(0);
            break;

        // toggle enable state for other keyboard buttons which are to be
        // continuously applied (like rotations, translations, ...)
        default:
            clothSimulator->keyboardStatus[key] = true;
    }
}

void normalKeyboardRelease(unsigned char key, int x, int y)
{
    clothSimulator->keyboardStatus[key] = false;
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            clothSimulator->showHelp();
            break;
        case GLUT_KEY_F2:
            clothSimulator->resetCameraPosition();
            break;
        case GLUT_KEY_F3:
            clothSimulator->showCameraStatus();
            break;
        case GLUT_KEY_F4:
            clothSimulator->showDrawStatus();
            break;
        case GLUT_KEY_F5:
            clothSimulator->drawStructuralConstraintsEnabled = !clothSimulator->drawStructuralConstraintsEnabled;
            break;
        case GLUT_KEY_F6:
            clothSimulator->drawShearConstraintsEnabled = !clothSimulator->drawShearConstraintsEnabled;
            break;
        case GLUT_KEY_F7:
            clothSimulator->drawStructuralBendConstraintsEnabled = !clothSimulator->drawStructuralBendConstraintsEnabled;
            break;
        case GLUT_KEY_F8:
            clothSimulator->drawShearBendConstraintsEnabled = !clothSimulator->drawShearBendConstraintsEnabled;
            break;
        case GLUT_KEY_F9:
            clothSimulator->drawNodesEnabled = !clothSimulator->drawNodesEnabled;
            break;
        case GLUT_KEY_F10:
            clothSimulator->drawWireFrameEnabled = !clothSimulator->drawWireFrameEnabled;
            break;
        case GLUT_KEY_F11:
            clothSimulator->drawWorldAxisEnabled = !clothSimulator->drawWorldAxisEnabled;
            break;
        case GLUT_KEY_F12:
            clothSimulator->drawSpheresEnabled = !clothSimulator->drawSpheresEnabled;
            break;

        default:
            break;
    }
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
    gluPerspective(60.0, aspectRatio, clothSimulator->nearPlane, clothSimulator->farPlane);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}