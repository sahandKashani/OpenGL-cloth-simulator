#include "ClothSimulator.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

ClothSimulator* ClothSimulator::instance = 0;

ClothSimulator* ClothSimulator::getInstance()
{
    if(instance == 0)
    {
        instance = new ClothSimulator();
    }

    return instance;
}

ClothSimulator::ClothSimulator() :
    nearPlane(1.0),
    farPlane(1000.0),
    drawWireFrameEnabled(false),
    drawNodesEnabled(false),
    drawWorldAxisEnabled(true),
    drawStructuralConstraintsEnabled(true),
    drawShearConstraintsEnabled(false),
    drawStructuralBendConstraintsEnabled(false),
    drawShearBendConstraintsEnabled(false),
    angleIncrement(0.03125),
    translationIncrement(1.0)
{}

ClothSimulator::~ClothSimulator()
{
    delete camera;
    delete cloth;
}

void ClothSimulator::createBatmanScene()
{
    cloth = new Cloth(21, 21);
    camera = new Camera();

    timeStep = 0.01;

    resetCameraPosition();

    // TODO : enable later
    // show help at program launch
    // showHelp();

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
}

void ClothSimulator::createScene()
{
    // clear keyboard press status
    initializeKeyboardStatus();

    createBatmanScene();
}

float ClothSimulator::getTimeStep()
{
    return timeStep;
}

// prints "true" if controlVariableEnabled is true, and "false" otherwise
std::string ClothSimulator::isEnabled(bool controlVariableEnabled)
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

void ClothSimulator::showHelp()
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

void ClothSimulator::showDrawStatus()
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

void ClothSimulator::showCameraStatus()
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

void ClothSimulator::resetCameraPosition()
{
    // reset the camera
    delete camera;
    camera = new Camera();

    // move camera back
    float cameraZ = 1.75 * std::max(cloth->getNumberNodesWidth(), cloth->getNumberNodesHeight());
    camera->translate(Vector3(0.0, 0.0, cameraZ));
}

// determines if a wireframe is to be drawn, or a textured version
void ClothSimulator::chooseRenderingMethod()
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

void ClothSimulator::drawWorldAxis()
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

void ClothSimulator::initializeKeyboardStatus()
{
    for(int i = 0; i < 256; i += 1)
    {
        keyboardStatus[i] = false;
    }
}

void ClothSimulator::applyContinuousKeyboardCommands()
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