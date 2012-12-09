#include "ClothSimulator.h"
#include "Keyboard.h"

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

ClothSimulator::ClothSimulator()
// :
    // nearPlane(1.0),
    // farPlane(1000.0),
    // drawWireFrameEnabled(true),
    // drawNodesEnabled(true),
    // drawWorldAxisEnabled(true),
    // drawStructuralConstraintsEnabled(true),
    // drawShearConstraintsEnabled(true),
    // drawStructuralBendConstraintsEnabled(true),
    // drawShearBendConstraintsEnabled(true),
    // drawSpheresEnabled(true),
    // angleIncrement(0.03125),
    // translationIncrement(0.125)
{}

// ClothSimulator::~ClothSimulator()
// {
//     delete camera;
//     delete cloth;
// }

void ClothSimulator::simulate()
{
//     // find out what commands the keyboard is sending and apply them
//     applyContinuousKeyboardCommands();

//     if(leftFootUp)
//     {
//         swingRightFoot();
//     } else
//     {
//         swingLeftFoot();
//     }

//     // apply all forces to the cloth
//     cloth->applyForces(getTimeStep());

//     // satisfy all constraints of the cloth after forces are applied
//     cloth->satisfyConstraints();

//     // handle all collisions with the spheres in the scene
//     cloth->handleSphereIntersections();
    Keyboard::getInstance()->applyNormalKeyboardActions();
    scene->simulate();
}

// void ClothSimulator::drawCloth()
// {
//     cloth->draw();
// }

// void ClothSimulator::drawFloor()
// {
//     floor->draw();
// }

void ClothSimulator::draw()
{
    // chooseRenderingMethod();

    // drawCloth();
    // drawTriangles();
    // drawFloor();

    // if(drawSpheresEnabled)
    // {
    //     drawSpheres();
    // }

    // if(drawWorldAxisEnabled)
    // {
    //     drawWorldAxis();
    // }

    scene->draw();
}

// void ClothSimulator::drawTriangles()
// {
//     for(std::vector<Triangle>::iterator triangleIterator = triangles.begin();
//         triangleIterator != triangles.end();
//         ++triangleIterator)
//     {
//         triangleIterator->draw();
//     }
// }

// void ClothSimulator::drawSpheres()
// {
//     for(std::vector<Sphere>::iterator sphereIterator = spheres.begin();
//         sphereIterator != spheres.end();
//         ++sphereIterator)
//     {
//         sphereIterator->draw();
//     }
// }

// void ClothSimulator::swingLeftFoot()
// {
//     Sphere* s = &spheres[0];
//     Vector3 center = s->getCenter();

//     if(center.z > 2.0){
//         leftFootUp = true;
//     }

//     if(leftFootUp)
//     {
//         s->setCenter(center + Vector3(0.0, -0.01, -0.05));
//         if(center.z <= 0.0)
//         {
//             leftFootUp = false;
//         }
//     }
//     else
//     {
//         s->setCenter(center + Vector3(0.0, 0.01, 0.05));
//     }
// }

// void ClothSimulator::swingRightFoot()
// {
//     Sphere* s = &spheres[1];
//     Vector3 center = s->getCenter();

//     if(center.z > 2.0){
//         rightFootUp = true;
//     }

//     if(rightFootUp)
//     {
//         s->setCenter(center + Vector3(0.0, -0.01, -0.05));
//         if(center.z <= 0.0)
//         {
//             rightFootUp = false;
//         }
//     }
//     else
//     {
//         s->setCenter(center + Vector3(0.0, 0.01, 0.05));
//     }
// }

void ClothSimulator::createScene()
{
    // camera = new Camera();

    // TODO : enable later
    // showHelp();

    // clear keyboard press status
    // initializeKeyboardStatus();
    Keyboard::getInstance()->resetKeyboardStatus();

    // createBatmanScene();

    scene = new BatmanScene();
}

// float ClothSimulator::getTimeStep()
// {
//     return timeStep;
// }

// // prints "true" if controlVariableEnabled is true, and "false" otherwise
// std::string ClothSimulator::isEnabled(bool controlVariableEnabled)
// {
//     if(controlVariableEnabled)
//     {
//         return "true";
//     }
//     else
//     {
//         return "false";
//     }
// }

// void ClothSimulator::showHelp()
// {
//     std::cout << "******************************************" << std::endl;
//     std::cout << "********           help           ********" << std::endl;
//     std::cout << "******************************************" << std::endl;

//     std::cout << std::endl;

//     std::cout << "ESC: exit cloth simulator" << std::endl;

//     std::cout << std::endl;

//     // help controls
//     std::cout << "help controls:" << std::endl;
//     std::cout << "  F1: show help" << std::endl;

//     std::cout << std::endl;

//     std::cout << "camera position reset:" << std::endl;
//     std::cout << "  F2: reset camera position" << std::endl;

//     std::cout << std::endl;

//     std::cout << "status controls:" << std::endl;
//     std::cout << "  F3: show camera status" << std::endl;
//     std::cout << "  F4: show draw   status" << std::endl;

//     std::cout << std::endl;

//     // drawing controls
//     std::cout << "drawing controls:" << std::endl;
//     std::cout << "  F5 : toggle draw structural      constraints" << std::endl;
//     std::cout << "  F6 : toggle draw shear           constraints" << std::endl;
//     std::cout << "  F7 : toggle draw structural bend constraints" << std::endl;
//     std::cout << "  F8 : toggle draw shear      bend constraints" << std::endl;
//     std::cout << "  F9 : toggle draw nodes" << std::endl;
//     std::cout << "  F10: toggle draw wireframe" << std::endl;
//     std::cout << "  F11: toggle draw world axis" << std::endl;
//     std::cout << "  F12: toggle draw spheres" << std::endl;

//     std::cout << std::endl;

//     // yaw, pitch and roll controls
//     std::cout << "camera object rotation controls:" << std::endl;
//     std::cout << "  j: yaw   left " << std::endl;
//     std::cout << "  l: yaw   right" << std::endl;
//     std::cout << "  k: pitch down " << std::endl;
//     std::cout << "  i: pitch up   " << std::endl;
//     std::cout << "  u: roll  left " << std::endl;
//     std::cout << "  o: roll  right" << std::endl;

//     std::cout << std::endl;

//     // world camera controls
//     std::cout << "camera world rotation controls:" << std::endl;
//     std::cout << "  s: rotate camera left  around world X axis" << std::endl;
//     std::cout << "  w: rotate camera right around world X axis" << std::endl;
//     std::cout << "  a: rotate camera left  around world Y axis" << std::endl;
//     std::cout << "  d: rotate camera right around world Y axis" << std::endl;
//     std::cout << "  e: rotate camera left  around world Z axis" << std::endl;
//     std::cout << "  q: rotate camera right around world Z axis" << std::endl;

//     std::cout << std::endl;

//     // camera position controls
//     std::cout << "camera translation controls:" << std::endl;
//     std::cout << "  f: translate camera left" << std::endl;
//     std::cout << "  h: translate camera right" << std::endl;
//     std::cout << "  t: translate camera up" << std::endl;
//     std::cout << "  g: translate camera down" << std::endl;
//     std::cout << "  r: translate camera back" << std::endl;
//     std::cout << "  z: translate camera front" << std::endl;

//     std::cout << std::endl;
// }

// void ClothSimulator::showDrawStatus()
// {
//     std::cout << "draw status:" << std::endl;
//     std::cout << "  draw nodes                      : " << isEnabled(drawNodesEnabled) << std::endl;
//     std::cout << "  draw structural constraints     : " << isEnabled(drawStructuralConstraintsEnabled) << std::endl;
//     std::cout << "  draw shear constraints          : " << isEnabled(drawShearConstraintsEnabled) << std::endl;
//     std::cout << "  draw structural bend constraints: " << isEnabled(drawStructuralBendConstraintsEnabled) << std::endl;
//     std::cout << "  draw shear bend constraints     : " << isEnabled(drawShearBendConstraintsEnabled) << std::endl;
//     std::cout << "  draw wireframe                  : " << isEnabled(drawWireFrameEnabled) << std::endl;
//     std::cout << "  draw world axis                 : " << isEnabled(drawWorldAxisEnabled) << std::endl;

//     std::cout << std::endl;
// }

// // determines if a wireframe is to be drawn, or a textured version
// void ClothSimulator::chooseRenderingMethod()
// {
//     if(drawWireFrameEnabled)
//     {
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//     }
//     else
//     {
//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//     }
// }

// void ClothSimulator::drawWorldAxis()
// {
//     glColor3f(1.0, 0.0, 0.0);
//     // x-axis in red
//     glBegin(GL_LINES);
//         glVertex3f(0.0, 0.0, 0.0);
//         glVertex3f(1.0, 0.0, 0.0);
//     glEnd();
//     glPushMatrix();
//         glRotatef(90, 0.0, 1.0, 0.0);
//         glTranslatef(0.0, 0.0, 1.0);
//         glutSolidCone(0.15, 0.30, 10, 10);
//     glPopMatrix();

//     // y-axis in green
//     glColor3f(0.0, 1.0, 0.0);
//     glBegin(GL_LINES);
//         glVertex3f(0.0, 0.0, 0.0);
//         glVertex3f(0.0, 1.0, 0.0);
//     glEnd();
//     glPushMatrix();
//         glRotatef(-90, 1.0, 0.0, 0.0);
//         glTranslatef(0.0, 0.0, 1.0);
//         glutSolidCone(0.15, 0.30, 10, 10);
//     glPopMatrix();

//     // z-axis in blue
//     glColor3f(0.0, 0.0, 1.0);
//     glBegin(GL_LINES);
//         glVertex3f(0.0, 0.0, 0.0);
//         glVertex3f(0.0, 0.0, 1.0);
//     glEnd();
//     glPushMatrix();
//         glTranslatef(0.0, 0.0, 1.0);
//         glutSolidCone(0.15, 0.30, 10, 10);
//     glPopMatrix();
// }

Scene* ClothSimulator::getScene()
{
    return scene;
}