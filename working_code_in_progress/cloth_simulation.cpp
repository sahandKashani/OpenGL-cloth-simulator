// compile with the following command:
//     clear; g++ -o cloth_simulation cloth_simulation.cpp Node.cpp Camera.cpp Constraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp -lglut -lGLU -lGL; ./cloth_simulation

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "settings.h"

#include "Node.h"
#include "Camera.h"
#include "Constraint.h"
#include "Arrow.h"
#include "Sphere.h"
#include "Cloth.h"

// Global variables
float nearPlane = 1.0;
float farPlane  = 1000.0;

bool drawWireFrameEnabled                 = false;
bool drawNodesEnabled                     = false;
bool drawWorldAxisEnabled                 = true;
bool drawStructuralConstraintsEnabled     = true;
bool drawShearConstraintsEnabled          = false;
bool drawStructuralBendConstraintsEnabled = false;
bool drawShearBendConstraintsEnabled      = false;

float angleIncrement = 0.03125; // 2^(-5)
float translationIncrement = 1.0;

char keyboardStatus[256];

struct timeval oldTime;

Cloth* cloth = 0;

Camera* camera = 0;

// -----------------------------------------------------------------------------
// GLUT setup

// prototypes
std::string isEnabled(bool controlVariableEnabled);
void showHelp();
void showDrawStatus();
void showCameraStatus();
void createScene();
void chooseRenderingMethod();
void drawWorldAxis();
void display();
void initializeKeyboardStatus();
void applyContinuousKeyboardCommands();
void normalKeyboard(unsigned char key, int x, int y);
void normalKeyboardRelease(unsigned char key, int x, int y);
void reshape(int w, int h);
void applyChanges();
void resetCameraPosition();
void specialKeyboard(int key, int x, int y);
float getTimeDifference();

void createScene()
{
    cloth = new Cloth(21, 21);
    camera = new Camera();

    resetCameraPosition();

    // TODO : enable later
    // show help at program launch
    // showHelp();

    // initialize the time (needed for future animations)
    // the value of oldTime will be changed through it's pointer
    gettimeofday(&oldTime, NULL);

    // TODO : fixing cloth at certain points
    cloth->setNodeMoveable(0, cloth->getNumberNodesHeight() - 1, false);
    cloth->setNodeMoveable(cloth->getNumberNodesWidth() / 2, cloth->getNumberNodesHeight() - 1, false);
    cloth->setNodeMoveable(cloth->getNumberNodesWidth() - 1, cloth->getNumberNodesHeight() - 1, false);

    // TODO : find suitable values
    // gravity
    Vector3 gravity(0.0, -1.0, 0.0);

    // TODO : find suitable values
    // wind
    Vector3 wind(0.0, 0.0, 0.5);

    cloth->addForce(gravity);
    cloth->addForce(wind);

    // clear keyboard press status
    initializeKeyboardStatus();
}

float getTimeDifference()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    struct timeval timeDifference;
    timeDifference.tv_sec = currentTime.tv_sec - oldTime.tv_sec;
    timeDifference.tv_usec = currentTime.tv_usec - oldTime.tv_usec;

    float duration = timeDifference.tv_sec + timeDifference.tv_usec / 1000000.0;

    // update oldTime for future time checks
    oldTime = currentTime;

    return duration;
}

// used for idle callback.
// Will process all changes that occur, such as keyboard commands, new forces, ...
void applyChanges()
{
    // find out what commands the keyboard is sending and apply them
    applyContinuousKeyboardCommands();

    // calculate time difference from last call to applyChanges()
    float duration = getTimeDifference();

    // apply all forces to the cloth
    cloth->applyForces(duration);

    // satisfy all constraints of the cloth after forces are applied
    cloth->satisfyConstraints();

    // redraw the screen
    glutPostRedisplay();
}

// prints "true" if controlVariableEnabled is true, and "false" otherwise
std::string isEnabled(bool controlVariableEnabled)
{
    if(controlVariableEnabled)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

void showHelp()
{
    std::cout << "******************************************" << std::endl;
    std::cout << "********           help           ********" << std::endl;
    std::cout << "******************************************" << std::endl;

    std::cout << std::endl;

    std::cout << "ESC: exit cloth simulator" << std::endl;

    std::cout << std::endl;

    // help controls
    std::cout << "help controls:" << std::endl;
    std::cout << "  F1: show help" << std::endl;

    std::cout << std::endl;

    std::cout << "camera position reset:" << std::endl;
    std::cout << "  F2: reset camera position" << std::endl;

    std::cout << std::endl;

    std::cout << "status controls:" << std::endl;
    std::cout << "  F3: show camera status" << std::endl;
    std::cout << "  F4: show draw   status" << std::endl;

    std::cout << std::endl;

    // drawing controls
    std::cout << "drawing controls:" << std::endl;
    std::cout << "  F5 : toggle draw structural      constraints" << std::endl;
    std::cout << "  F6 : toggle draw shear           constraints" << std::endl;
    std::cout << "  F7 : toggle draw structural bend constraints" << std::endl;
    std::cout << "  F8 : toggle draw shear      bend constraints" << std::endl;
    std::cout << "  F9 : toggle draw nodes" << std::endl;
    std::cout << "  F10: toggle draw wireframe" << std::endl;
    std::cout << "  F11: toggle draw world axis" << std::endl;

    std::cout << std::endl;

    // yaw, pitch and roll controls
    std::cout << "camera object rotation controls:" << std::endl;
    std::cout << "  j: yaw   left " << std::endl;
    std::cout << "  l: yaw   right" << std::endl;
    std::cout << "  k: pitch down " << std::endl;
    std::cout << "  i: pitch up   " << std::endl;
    std::cout << "  u: roll  left " << std::endl;
    std::cout << "  o: roll  right" << std::endl;

    std::cout << std::endl;

    // world camera controls
    std::cout << "camera world rotation controls:" << std::endl;
    std::cout << "  s: rotate camera left  around world X axis" << std::endl;
    std::cout << "  w: rotate camera right around world X axis" << std::endl;
    std::cout << "  a: rotate camera left  around world Y axis" << std::endl;
    std::cout << "  d: rotate camera right around world Y axis" << std::endl;
    std::cout << "  e: rotate camera left  around world Z axis" << std::endl;
    std::cout << "  q: rotate camera right around world Z axis" << std::endl;

    std::cout << std::endl;

    // camera position controls
    std::cout << "camera translation controls:" << std::endl;
    std::cout << "  f: translate camera left" << std::endl;
    std::cout << "  h: translate camera right" << std::endl;
    std::cout << "  t: translate camera up" << std::endl;
    std::cout << "  g: translate camera down" << std::endl;
    std::cout << "  r: translate camera back" << std::endl;
    std::cout << "  z: translate camera front" << std::endl;

    std::cout << std::endl;
}

void showDrawStatus()
{
    std::cout << "draw status:" << std::endl;
    std::cout << "  draw nodes                      : " << isEnabled(drawNodesEnabled) << std::endl;
    std::cout << "  draw structural constraints     : " << isEnabled(drawStructuralConstraintsEnabled) << std::endl;
    std::cout << "  draw shear constraints          : " << isEnabled(drawShearConstraintsEnabled) << std::endl;
    std::cout << "  draw structural bend constraints: " << isEnabled(drawStructuralBendConstraintsEnabled) << std::endl;
    std::cout << "  draw shear bend constraints     : " << isEnabled(drawShearBendConstraintsEnabled) << std::endl;
    std::cout << "  draw wireframe                  : " << isEnabled(drawWireFrameEnabled) << std::endl;
    std::cout << "  draw world axis                 : " << isEnabled(drawWorldAxisEnabled) << std::endl;

    std::cout << std::endl;
}

void showCameraStatus()
{
    std::cout << "camera status:" << std::endl;
    std::cout << "  camera position      : " << camera->getPosition().toString() << std::endl;
    std::cout << "  camera view direction: " << camera->getViewDirection().toString() << std::endl;
    std::cout << "  camera up direction  : " << camera->getUpDirection().toString() << std::endl;
    std::cout << "  yaw                  : " << camera->getYaw() << " rad" << std::endl;
    std::cout << "  pitch                : " << camera->getPitch() << " rad" << std::endl;
    std::cout << "  roll                 : " << camera->getRoll() << " rad" << std::endl;

    std::cout << std::endl;
}

void resetCameraPosition()
{
    // reset the camera
    delete camera;
    camera = new Camera();

    // move camera back
    float cameraZ = 1.75 * std::max(cloth->getNumberNodesWidth(), cloth->getNumberNodesHeight());
    camera->translate(Vector3(0.0, 0.0, cameraZ));
}

// determines if a wireframe is to be drawn, or a textured version
void chooseRenderingMethod()
{
    if(drawWireFrameEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void drawWorldAxis()
{
    if(drawWorldAxisEnabled)
    {
        glColor3f(1.0, 0.0, 0.0);
        // x-axis in red
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
        glEnd();
        glPushMatrix();
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();

        // y-axis in green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);
        glEnd();
        glPushMatrix();
            glRotatef(-90, 1.0, 0.0, 0.0);
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();

        // z-axis in blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 1.0);
        glEnd();
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();
    }
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

    Vector3 cameraPosition = camera->getPosition();
    Vector3 cameraViewDirection = camera->getViewDirection();
    Vector3 cameraUpDirection = camera->getUpDirection();

    // all perspectives are already calculated and stored, so just position
    // the camera where it is supposed to be
    gluLookAt(cameraPosition.x,
              cameraPosition.y,
              cameraPosition.z,
              cameraViewDirection.x,
              cameraViewDirection.y,
              cameraViewDirection.z,
              cameraUpDirection.x,
              cameraUpDirection.y,
              cameraUpDirection.z);

    // choose wireframe or solid rendering
    chooseRenderingMethod();

    // draw the world axis
    drawWorldAxis();

    // draw cloth
    cloth->draw();

    // swap buffers needed for double buffering
    glutSwapBuffers();
}

void initializeKeyboardStatus()
{
    for(int i = 0; i < 256; i += 1)
    {
        keyboardStatus[i] = false;
    }
}

void applyContinuousKeyboardCommands()
{
    for(int key = 0; key < 256; key += 1)
    {
        // if the key is being continuously held down
        if(keyboardStatus[key])
        {
            switch(key)
            {
                // world axis camera controls
                case 'a':
                    camera->rotateAroundYAxisWorld(-angleIncrement);
                    break;
                case 'd':
                    camera->rotateAroundYAxisWorld(angleIncrement);
                    break;
                case 's':
                    camera->rotateAroundXAxisWorld(-angleIncrement);
                    break;
                case 'w':
                    camera->rotateAroundXAxisWorld(angleIncrement);
                    break;
                case 'q':
                    camera->rotateAroundZAxisWorld(-angleIncrement);
                    break;
                case 'e':
                    camera->rotateAroundZAxisWorld(angleIncrement);
                    break;

                // yaw, pitch and roll camera controls
                case 'k':
                    // turn camera "down" one notch
                    camera->rotateAroundXAxisObject(angleIncrement);
                    break;
                case 'i':
                    // turn camera "up" one notch
                    camera->rotateAroundXAxisObject(-angleIncrement);
                    break;
                case 'j':
                    // turn camera "left" one notch
                    camera->rotateAroundYAxisObject(angleIncrement);
                    break;
                case 'l':
                    // turn camera "right" one notch
                    camera->rotateAroundYAxisObject(-angleIncrement);
                    break;
                case 'u':
                    // tilt camera "left" one notch
                    camera->rotateAroundZAxisObject(-angleIncrement);
                    break;
                case 'o':
                    // tilt camera "right" one notch
                    camera->rotateAroundZAxisObject(angleIncrement);
                    break;

                // camera translation controls
                case 'f':
                    // translate camera left
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    // FIXME : translates in wrong direction
                    camera->translate(camera->getUpDirection().cross(camera->getViewDirection()).normalize() * translationIncrement);
                    break;
                case 'h':
                    // translate camera right
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    // FIXME : translates in wrong direction
                    camera->translate(camera->getViewDirection().cross(camera->getUpDirection()).normalize() * translationIncrement);
                    break;
                case 't':
                    // translate camera up
                    camera->translate(camera->getUpDirection().normalize() * translationIncrement);
                    break;
                case 'g':
                    // translate camera down
                    camera->translate(-camera->getUpDirection().normalize() * translationIncrement);
                    break;
                case 'r':
                    // translate camera back
                    // FIXME : gets stuck if advance too much, since the vector's length becomes 0.0, and normalize throws assertion error
                    camera->translate(camera->getViewDirection().normalize() * translationIncrement);
                    break;
                case 'z':
                    // translate camera front
                    // FIXME : gets stuck if advance too much, since the vector's length becomes 0.0, and normalize throws assertion error
                    camera->translate(-camera->getViewDirection().normalize() * translationIncrement);
                    break;

                default:
                    break;
            } // end switch(key)
        } // end if(keyboardStatus[key])
    } // end loop
}

void normalKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        // ESC control
        case 27:
            // clean up the cloth and camera before exiting
            delete camera;
            delete cloth;
            exit(0);
            break;

        // toggle enable state for other keyboard buttons which are to be
        // continuously applied (like rotations, translations, ...)
        default:
            keyboardStatus[key] = true;
    }
}

void normalKeyboardRelease(unsigned char key, int x, int y)
{
    keyboardStatus[key] = false;
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            showHelp();
            break;
        case GLUT_KEY_F2:
            resetCameraPosition();
            break;
        case GLUT_KEY_F3:
            showCameraStatus();
            break;
        case GLUT_KEY_F4:
            showDrawStatus();
            break;
        case GLUT_KEY_F5:
            drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
            break;
        case GLUT_KEY_F6:
            drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
            break;
        case GLUT_KEY_F7:
            drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
            break;
        case GLUT_KEY_F8:
            drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
            break;
        case GLUT_KEY_F9:
            drawNodesEnabled = !drawNodesEnabled;
            break;
        case GLUT_KEY_F10:
            drawWireFrameEnabled = !drawWireFrameEnabled;
            break;
        case GLUT_KEY_F11:
            drawWorldAxisEnabled = !drawWorldAxisEnabled;
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
    gluPerspective(60.0, aspectRatio, nearPlane, farPlane);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(1200, 800);

    glutCreateWindow("Batman");

    createScene();

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