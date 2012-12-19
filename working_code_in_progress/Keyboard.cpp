#include "Keyboard.h"
#include "Camera.h"
#include "ClothSimulator.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

Keyboard* Keyboard::instance = 0;

Keyboard* Keyboard::getInstance()
{
    if(instance == 0)
    {
        instance = new Keyboard();
    }

    return instance;
}

Keyboard::Keyboard() :
    angleIncrement(0.015625),    // 2^(-6)
    translationIncrement(0.125), // 2^(-3)
    spacebarPressed(false)
{}

void Keyboard::resetKeyboardStatus()
{
    for(int i = 0; i < 256; i += 1)
    {
        keyboardStatus[i] = false;
    }
}

// Note that these are the CONTINUOUS actions only. For toggle state actions,
// take care of it before the array of keyboard button status is enabled.
void Keyboard::applyNormalKeyboardActions()
{
    Camera* camera = ClothSimulator::getInstance()->getScene()->getCamera();

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
                    camera->translate(camera->getUpDirection().cross(camera->getViewDirection()).normalize() * translationIncrement);
                    break;
                case 'h':
                    // translate camera right
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
                    camera->translate(-camera->getViewDirection().normalize() * translationIncrement);
                    break;
                case 'z':
                    // translate camera front
                    camera->translate(camera->getViewDirection().normalize() * translationIncrement);
                    break;

                default:
                    break;
            } // end switch(key)
        } // end if(keyboardStatus[key])
    } // end loop
}

// Put toggle keyboard controls here, and continuous ones in the "default" part of
// the switch statement
void Keyboard::handleNormalKeyboardInput(unsigned char key, int x, int y)
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    switch(key)
    {
        case '1':
            drawingSettings->toggleDrawArrowsEnabled();
            break;
        case '2':
            drawingSettings->toggleDrawFloorEnabled();
            break;
        case '3':
            drawingSettings->toggleDrawTrianglesEnabled();
            break;
        case 32:
            spacebarPressed = !spacebarPressed;

            if(spacebarPressed)
            {
                drawingSettings->setTimeStep(drawingSettings->getOriginalTimeStep());
            }
            else
            {
                drawingSettings->setTimeStep(0.0);
            }

        default:
            // toggle enable state for keyboard buttons which are to be
            // continuously applied (like rotations, translations, ...)
            keyboardStatus[key] = true;
            break;
    }

}

void Keyboard::handleNormalKeyboardRelease(unsigned char key, int x, int y)
{
    // toggle disable state for keyboard buttons which are to be
    // continuously applied (like rotations, translations, ...)
    keyboardStatus[key] = false;
}

void Keyboard::handleSpecialKeyboardInput(int key, int x, int y)
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    switch(key)
    {
        case GLUT_KEY_F1:
            showHelp();
            break;
        case GLUT_KEY_F2:
            ClothSimulator::getInstance()->getScene()->getCamera()->loadCameraSetup();
            break;
        case GLUT_KEY_F3:
            ClothSimulator::getInstance()->getScene()->getCamera()->showCameraStatus();
            break;
        case GLUT_KEY_F4:
            DrawingSettings::getInstance()->showDrawStatus();
            break;
        case GLUT_KEY_F5:
            drawingSettings->toggleDrawStructuralConstraintsEnabled();
            break;
        case GLUT_KEY_F6:
            drawingSettings->toggleDrawShearConstraintsEnabled();
            break;
        case GLUT_KEY_F7:
            break;
        case GLUT_KEY_F8:
            break;
        case GLUT_KEY_F9:
            drawingSettings->toggleDrawNodesEnabled();
            break;
        case GLUT_KEY_F10:
            drawingSettings->toggleDrawWireFrameEnabled();
            break;
        case GLUT_KEY_F11:
            drawingSettings->toggleDrawWorldAxisEnabled();
            break;
        case GLUT_KEY_F12:
            drawingSettings->toggleDrawSpheresEnabled();
            break;

        default:
            break;
    }
}

void Keyboard::showHelp()
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
    std::cout << "  F5   : toggle draw structural      constraints" << std::endl;
    std::cout << "  F6   : toggle draw shear           constraints" << std::endl;
    std::cout << "  F7   : toggle draw structural bend constraints" << std::endl;
    std::cout << "  F8   : toggle draw shear      bend constraints" << std::endl;
    std::cout << "  F9   : toggle draw nodes" << std::endl;
    std::cout << "  F10  : toggle draw wireframe" << std::endl;
    std::cout << "  F11  : toggle draw world axis" << std::endl;
    std::cout << "  F12  : toggle draw spheres" << std::endl;
    std::cout << "  1    : toggle draw arrows" << std::endl;
    std::cout << "  2    : toggle draw floor" << std::endl;
    std::cout << "  3    : toggle draw triangles" << std::endl;
    std::cout << "  space: toggle pause" << std::endl;

    std::cout << std::endl;

    // yaw, pitch and roll camera controls
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