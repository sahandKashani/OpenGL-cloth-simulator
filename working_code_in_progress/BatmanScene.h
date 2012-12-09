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
    void createScene();
    Cloth* cape;
    Floor* floor;

    std::vector<Sphere> leftFoot;
    std::vector<Sphere> rightFoot;

public:
    BatmanScene();
    void draw();
    void drawLeftFoot();
    void drawRightFoot();
};

#endif