#include "BatmanScene.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

BatmanScene::BatmanScene() :
    Scene()
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
    // camera->setPosition(Vector3(-10.0, 7.5, 20.0));
    // camera->setViewDirection(Vector3(1.0, 0.0, -1.0));
    camera->setPosition(Vector3(5.0, 5.0, -30.0));
    camera->setViewDirection(Vector3(0.0, 0.0, 1.0));
    camera->setUpDirection(Vector3(0.0, 1.0, 0.0));
    camera->saveCameraSetup();

    // cloth setup (cannot put more than 7 rigidity)
    cape = new Cloth(10.0, 15.0, 15, 2);

    // fixing cape at certain points
    // cape->getNode(0,0)->setMoveable(false);
    cape->getNode(0                              , cape->getNumberNodesHeight() - 1)->setMoveable(false);
    cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() - 1)->setMoveable(false);

    // setting cape node mass
    for(int x = 0; x < cape->getNumberNodesWidth() - 1; x += 1)
    {
        for(int y = 0; y < cape->getNumberNodesHeight() - 1; y += 1)
        {
            cape->getNode(x, y)->setMass(1.0);
        }
    }

    // feet setup
    Vector3 centerBetweenFeet(5.0, 2.0, -2.0);
    // left foot
    leftFoot.push_back(Sphere(centerBetweenFeet  + Vector3(-2.0, 0.0, 0.0), 1.2));
    // right foot
    rightFoot.push_back(Sphere(centerBetweenFeet + Vector3( 2.0, 0.0, 0.0), 1.2));

    // body setup
    Vector3 bodyCenter(5.0, 10, -1.0);
    body.push_back(Sphere(bodyCenter + Vector3(-2.0,  0.0, 0.0), 1.2));
    body.push_back(Sphere(bodyCenter + Vector3( 2.0,  0.0, 0.0), 1.2));
    body.push_back(Sphere(bodyCenter + Vector3( 0.0, -2.0, 0.0), 1.2));
    body.push_back(Sphere(bodyCenter + Vector3( 0.0,  2.0, 0.0), 1.2));

    // shoulder setup
    Vector3 shoulderHeightCenter((cape->getNode(0, cape->getNumberNodesHeight() - 1)->getPosition() +
                                  cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() - 1)->getPosition()) / 2.0 -
                                  Vector3(0.0, 1.0, 2.0));
    leftShoulder.push_back(Sphere(shoulderHeightCenter + Vector3(-2.0,  0.0, 0.0), 1.2));

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
    Vector3 gravity(0.0, -10.0, 0.0);
    // wind
    Vector3 wind(0.0, 0.0, 0.0);

    // add forces to cape
    // cape->addForce(gravity);
    cape->addForce(wind);

    time = 0.0;
}

void BatmanScene::simulate()
{
    float timeStep = 0.1;
    time += timeStep;
    cape->applyForces(timeStep);
    cape->satisfyConstraints();

    // swingLeftFoot();
    // swingRightFoot();

    swingLeftShoulder();
    swingRightShoulder();

    cape->handleSphereIntersections(&leftFoot);
    cape->handleSphereIntersections(&rightFoot);
    cape->handleSphereIntersections(&body);
    cape->handleSphereIntersections(&leftShoulder);
    cape->handleSphereIntersections(&rightShoulder);

    cape->handleSelfIntersections();
}

void BatmanScene::swingLeftShoulder()
{
    Node* left = cape->getNode(0, cape->getNumberNodesHeight() - 1);
    float r = 1.2;
    float t = time * 0.5;

    float x = left->getPosition().x;
    float y = left->getPosition().y;
    float z = -r * cos(t);

    Vector3 position(x, y, z);
    left->setPosition(position);
}

void BatmanScene::swingRightShoulder()
{
    Node* right = cape->getNode(cape->getNumberNodesWidth() - 1, cape->getNumberNodesHeight() - 1);
    float r = 1.2;
    float t = time * 0.5;

    float x = right->getPosition().x;
    float y = right->getPosition().y;
    float z = r * cos(t);

    Vector3 position(x, y, z);
    right->setPosition(position);
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

    // draw body
    drawBodyElement(&body);

    // draw shoulders
    drawBodyElement(&leftShoulder);
    drawBodyElement(&rightShoulder);
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
    float r = 1.2;
    float t = time;

    float x = left->getCenter().x;
    // float y = left->getCenter().y + r * sin(t);
    // float z = left->getCenter().z + r * cos(t);
    float y = r * sin(t);
    float z = left->getCenter().z + r * cos(t);

    Vector3 position(x, y, z);
    left->setCenter(position);
}

void BatmanScene::swingRightFoot()
{
    Sphere* right = &rightFoot[0];
    float r = 1.2;
    float t = time;
    float pi = 3.141592;

    float x = right->getCenter().x;
    float y = r * sin(t - pi);
    float z = r * cos(t - pi);

    Vector3 position(x, y, z);
    right->setCenter(position);
}