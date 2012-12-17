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
    std::vector<Sphere> leftShoulder;
    std::vector<Sphere> rightShoulder;
    std::vector<Sphere> body;

    float time;

    void createScene();
    void drawBodyElement(std::vector<Sphere>* elements);

    void swingLeftFoot();
    void swingRightFoot();

    void swingLeftShoulder();
    void swingRightShoulder();

public:
    BatmanScene();
    void draw();
    void simulate();
};

#endif