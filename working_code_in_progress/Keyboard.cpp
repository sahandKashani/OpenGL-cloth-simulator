#include "Keyboard.h"
#include "Camera.h"
#include "ClothSimulator.h"

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
    angleIncrement(0.03125),
    translationIncrement(0.125)
{}

void Keyboard::resetKeyboardStatus()
{
    for(int i = 0; i < 256; i += 1)
    {
        keyboardStatus[i] = false;
    }
}

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
                    camera->translate(-camera->getRightDirection().normalize() * translationIncrement);
                    break;
                case 'h':
                    // translate camera rightewDirection().cross(camera->getUpDirection()).normalize() * translationIncrement);
                    camera->translate(camera->getRightDirection().normalize() * translationIncrement);
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

void Keyboard::handleNormalKeyboardInput(unsigned char key, int x, int y)
{
    // toggle enable state for keyboard buttons which are to be
    // continuously applied (like rotations, translations, ...)
    keyboardStatus[key] = true;
}

void Keyboard::handleNormalKeyboardRelease(unsigned char key, int x, int y)
{
    // toggle disable state for keyboard buttons which are to be
    // continuously applied (like rotations, translations, ...)
    keyboardStatus[key] = false;
}

// void Keyboard::handleSpecialKeyboardInput(int key, int x, int y)
// {
//     switch(key)
//     {
//         case GLUT_KEY_F1:
//             showHelp();
//             break;
//         case GLUT_KEY_F2:
//             resetCameraPosition();
//             break;
//         case GLUT_KEY_F3:
//             showCameraStatus();
//             break;
//         case GLUT_KEY_F4:
//             showDrawStatus();
//             break;
//         case GLUT_KEY_F5:
//             drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
//             break;
//         case GLUT_KEY_F6:
//             drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
//             break;
//         case GLUT_KEY_F7:
//             drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
//             break;
//         case GLUT_KEY_F8:
//             drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
//             break;
//         case GLUT_KEY_F9:
//             drawNodesEnabled = !drawNodesEnabled;
//             break;
//         case GLUT_KEY_F10:
//             drawWireFrameEnabled = !drawWireFrameEnabled;
//             break;
//         case GLUT_KEY_F11:
//             drawWorldAxisEnabled = !drawWorldAxisEnabled;
//             break;
//         case GLUT_KEY_F12:
//             drawSpheresEnabled = !drawSpheresEnabled;
//             break;

//         default:
//             break;
//     }
// }