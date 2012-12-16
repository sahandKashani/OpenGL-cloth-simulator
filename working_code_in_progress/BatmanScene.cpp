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
    camera->setPosition(Vector3(-10.0, 7.5, 20.0));
    camera->setViewDirection(Vector3(1.0, 0.0, -1.0));
    camera->setUpDirection(Vector3(0.0, 1.0, 0.0));
    camera->saveCameraSetup();

    // cloth setup (cannot put more than 7 rigidity)
    cape = new Cloth(10.0, 15.0, 20, 2);
    // fixing cape at certain points
    cape->getNode(0                                    , cape->getNumberNodesHeight() - 1)->setMoveable(false);
    // cape->getNode((cape->getNumberNodesWidth() - 1) / 2, cape->getNumberNodesHeight() - 1)->setMoveable(false);
    cape->getNode(cape->getNumberNodesWidth() - 1      , cape->getNumberNodesHeight() - 1)->setMoveable(false);
    // setting cape node mass
    for(int x = 0; x < cape->getNumberNodesWidth() - 1; x += 1)
    {
        for(int y = 0; y < cape->getNumberNodesHeight() - 1; y += 1)
        {
            cape->getNode(x, y)->setMass(1.0);
        }
    }

    // body setup
    Vector3 centerBetweenFeet(5.0, 2.0, -1.0);
    // left foot
    leftFoot.push_back(Sphere(centerBetweenFeet + Vector3(-2.0, 0.0, 0.0 ), 1.2));
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
    Vector3 wind(0.0, 0.0, -0.1);

    // add forces to cape
    // cape->addForce(gravity);
    cape->addForce(wind);
}

void BatmanScene::simulate()
{
    float timeStep = 0.001;
    cape->applyForces(timeStep);
    cape->satisfyConstraints();
    swingLeftFoot();
    swingRightFoot();
    cape->handleSphereIntersections(&leftFoot);
    cape->handleSphereIntersections(&rightFoot);
    cape->handleSelfIntersections();
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

}

void BatmanScene::swingRightFoot()
{

}