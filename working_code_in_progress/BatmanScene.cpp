#include "BatmanScene.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

BatmanScene::BatmanScene() :
    Scene(),
    pi(3.141592)
{
    createScene();
}

void BatmanScene::createScene()
{
    GLfloat light_position[] = {0.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    nearPlane = 1.0;
    farPlane  = 200.0;

    // camera setup
    camera = new Camera();
    camera->setPosition(Vector3(-10.0, 7.5, 20.0));
    camera->setViewDirection(Vector3(1.0, 0.0, -1.0));
    camera->setUpDirection(Vector3(0.0, 1.0, 0.0));
    camera->saveCameraSetup();

    // cloth setup (cannot put more than 7 rigidity)
    cape = new Cloth(10.0, 15.0, 15, 7);

    // fixing cape at certain points
    cape->getNode(0                              , cape->getNumberNodesHeight() - 1)->setMoveable(false);
    cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() - 1)->setMoveable(false);
    // setting cape node mass
    for(int x = 0; x < cape->getNumberNodesWidth() - 1; x += 1)
    {
        for(int y = 0; y < cape->getNumberNodesHeight() - 1; y += 1)
        {
            cape->getNode(x, y)->setMass(1000000.0);
        }
    }

    // body setup
    Vector3 centerBetweenFeet(5.0, 2.0, -1.0);
    // left foot
    leftFoot.push_back(Sphere(centerBetweenFeet +  Vector3(-2.0, 0.0, 0.0 ), 1.2));
    // right foot
    rightFoot.push_back(Sphere(centerBetweenFeet + Vector3( 2.0, 0.0, 0.0 ), 1.2));

    // rightFoot.push_back(Sphere(Vector3(7.5, 12.0, 5.0), 1.5));
    // rightFoot.push_back(Sphere(Vector3(7.5, 10.0, 5.0), 1.5));
    // rightFoot.push_back(Sphere(Vector3(7.5, 8.0, 5.0) , 1.5));
    // rightFoot.push_back(Sphere(Vector3(7.5, 6.0, 5.0) , 1.5));
    // rightFoot.push_back(Sphere(Vector3(7.5, 4.0, 5.0) , 1.5));
    // rightFoot.push_back(Sphere(Vector3(7.5, 2.0, 5.0) , 1.5));

    // center ball collision test
    // rightFoot.push_back(Sphere(Vector3(7.5, 10.0, 5.0), 1.5));

    // forces
    // gravity
    Vector3 gravity(0.0, -1.0, 0.0);
    // wind
    Vector3 wind(0.0, 0.0, 1.0);

    // add forces to cape
    cape->addForce(gravity);
    cape->addForce(wind);

    time = 0.0;
}

void BatmanScene::simulate()
{
    float timeStep = 0.0005;
    time += timeStep;
    cape->applyForces(timeStep);
    cape->satisfyConstraints();

    swingLeftFoot();
    swingLeftShoulder();

    swingRightFoot();
    swingRightShoulder();

    Node* leftshoulder = cape->getNode(0, cape->getNumberNodesHeight() - 1);
    Node* rightshoulder = cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() -1);

    float r = 1.5;
    float t = time * 100;
    float z = -r * (t - sin(t));

    Vector3 direction(0.0, 0.0, z - leftshoulder->getPosition().z);
    leftshoulder->translate(direction);
    rightshoulder->translate(direction);

    cape->handleSphereIntersections(&leftFoot);
    cape->handleSphereIntersections(&rightFoot);
    cape->handleSelfIntersections();

    // followBatman();
}

void BatmanScene::followBatman()
{
    float r = 1.5;
    float t = time * 100;
    float z = -r * (t - sin(t));

    camera->translate(Vector3(0.0, 0.0, z - camera->getPosition().z));
}

void BatmanScene::draw()
{
    DrawingSettings::getInstance()->chooseRenderingMethod();

    drawWorldAxis();

    // draw cape
    cape->draw();

    // draw feet
    drawBodyElement(&leftFoot);
    drawBodyElement(&rightFoot);
}

void BatmanScene::drawBodyElement(std::vector<Sphere>* elements)
{
    for(std::vector<Sphere>::iterator sphereIterator = elements->begin();
        sphereIterator != elements->end();
        ++sphereIterator)
    {
        sphereIterator->draw();
    }
}

void BatmanScene::swingLeftFoot()
{
    Sphere* left = &leftFoot[0];
    float r = 1.5;
    float t = time * 100;

    float x = left->getCenter().x;
    float y = r  * (1 - cos(t));
    float z = -r * (t - sin(t));

    Vector3 position(x, y, z);
    left->setCenter(position);
}

void BatmanScene::swingRightFoot()
{
    Sphere* right = &rightFoot[0];
    float r = 1.5;
    float t = time * 100;

    float x = right->getCenter().x;
    float y = r  * (1 - cos(t - pi));
    float z = -r * (t - sin(t - pi));

    Vector3 position(x, y, z);
    right->setCenter(position);
}

void BatmanScene::swingLeftShoulder()
{
    Node* left = cape->getNode(0, cape->getNumberNodesHeight() - 1);
    float r = 1.5;
    float t = time * 100;

    float x = left->getPosition().x;
    float y = left->getPosition().y;
    float z = -r * (t - sin(t - pi));

    Vector3 position(x, y, z);
    left->setPosition(position);
}

void BatmanScene::swingRightShoulder()
{
    Node* right = cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() - 1);
    float r = 1.5;
    float t = time * 100;

    float x = right->getPosition().x;
    float y = right->getPosition().y;
    float z = -r * (t - sin(t));

    Vector3 position(x, y, z);
    right->setPosition(position);
}