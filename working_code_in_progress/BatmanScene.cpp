#include "BatmanScene.h"

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
    // create the scene here, along with all objects necessary
    nearPlane = 1.0;
    farPlane  = 200.0;

    camera = new Camera();

    // cloth instantiation
    cape = new Cloth(10.0, 15.0, 20, 30);
    floor = new Floor(Vector3(0.0, 0.0, 0.0),
                      Vector3(0.0, 0.0, 10.0),
                      Vector3(10.0, 0.0, 10.0),
                      Vector3(10.0, 0.0, 0.0));

    // camera setup
    camera->setPosition(Vector3(-10.0, 5.0, 20.0));
    camera->setViewDirection(Vector3(1.0, 0.0, -1.0));
    camera->setUpDirection(Vector3(0.0, 1.0, 0.0));

    // fixing cape at certain points
    cape->getNode(0                                    , cape->getNumberNodesHeight() - 1)->setMoveable(false);
    cape->getNode((cape->getNumberNodesWidth() - 1) / 2, cape->getNumberNodesHeight() - 1)->setMoveable(false);
    cape->getNode(cape->getNumberNodesWidth() - 1      , cape->getNumberNodesHeight() - 1)->setMoveable(false);

    // setting cape node mass
    for(int x = 0; x < cape->getNumberNodesWidth() - 1; x += 1)
    {
        for(int y = 0; y < cape->getNumberNodesHeight() - 1; y += 1)
        {
            cape->getNode(x, y)->setMass(4.0);
        }
    }

    Vector3 centerBetweenFeet(5.0, 2.0, -1.0);
    // left foot
    leftFoot.push_back(Sphere(centerBetweenFeet + Vector3(-2.0, 0.0, 0.0 ), 1.2, false));

    // right foot
    rightFoot.push_back(Sphere(centerBetweenFeet + Vector3( 2.0, 0.0, 0.0 ), 1.2, false));

    // forces
    // gravity
    Vector3 gravity(0.0, -10.0, 0.0);
    // wind
    Vector3 wind(0.0, 0.0, -0.2);

    // add forces to cape
    cape->addForce(gravity);
    cape->addForce(wind);
}

void BatmanScene::draw()
{
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