#ifndef BATMAN_SCENE_H
#define BATMAN_SCENE_H

#include "Scene.h"
#include "Cloth.h"
#include "Floor.h"
#include "Sphere.h"
#include <vector>

class BatmanScene : public Scene
{
private:
    Cloth* cape;
    Floor* floor;

    std::vector<Sphere> leftFoot;
    std::vector<Sphere> rightFoot;

    std::vector<Sphere> boundaries;

    std::vector<Sphere> otherSpheres;

    float pi;

    float time;

    void createScene();
    void drawBodyElement(std::vector<Sphere>* elements);

    void swingLeftFoot();
    void swingRightFoot();

    void swingLeftShoulder();
    void swingRightShoulder();

    void followBatman();

    void translateBoundaries();

    void setClothMass(float mass);
    void setupClothTips();
    void createBoundaries();
    void setupCamera();
    void setupFeet();

    void drawFeet();
    void drawOtherSpheres();
    void addForces();
    void createCenterCollisionBallScene();
    void createRunningScene();

    bool runningSceneEnabled;

public:
    BatmanScene();
    void draw();
    void simulate();
};

#endif