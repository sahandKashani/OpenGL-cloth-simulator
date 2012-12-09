#include "BatmanScene.h"

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

    // cloth instantiation
    cape = new Cloth(10.0, 15.0, 20, 30);
    floor = new Floor(Vector3(0.0, 0.0, 0.0),
                      Vector3(0.0, 0.0, 10.0),
                      Vector3(10.0, 0.0, 10.0),
                      Vector3(10.0, 0.0, 0.0));

    camera.setPosition(Vector3(-10.0, 5.0, -10.0));
    camera.setViewDirection(Vector3(5.0, 5.0, 0.0));
    camera.setUpDirection(Vector3(0.0, 1.0, 0.0));

    // // reset camera to center of cloth
    // resetCameraPosition();

    // // TODO : fixing cloth at certain points
    // cloth->getNode(0, cloth->getNumberNodesHeight() - 1)->setMoveable(false);
    // cloth->getNode((cloth->getNumberNodesWidth() - 1) / 2, cloth->getNumberNodesHeight() - 1)->setMoveable(false);
    // cloth->getNode(cloth->getNumberNodesWidth() - 1, cloth->getNumberNodesHeight() - 1)->setMoveable(false);
    // for(int x = 0; x < cloth->getNumberNodesWidth() - 1; x += 1)
    // {
    //     for(int y = 0; y < cloth->getNumberNodesHeight() - 1; y += 1)
    //     {
    //         cloth->getNode(x, y)->setMass(4.0);
    //     }
    // }

    // Vector3 center(5.0, 2.0, -1.0);
    // // left foot
    // leftFootUp = false;
    // spheres.push_back(Sphere(center + Vector3(-2.0, 0.0, 0.0 ), 1.2, false));

    // // right foot
    // rightFootUp = false;
    // spheres.push_back(Sphere(center + Vector3( 2.0, 0.0, 0.0 ), 1.2, false));

    // // TODO : find suitable values
    // // gravity
    // Vector3 gravity(0.0, -10.0, 0.0);

    // // TODO : find suitable values
    // // wind
    // Vector3 wind(0.0, 0.0, -0.2);

    // cloth->addForce(gravity);
    // cloth->addForce(wind);
}