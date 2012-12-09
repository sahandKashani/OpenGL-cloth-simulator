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

    void createScene();
    void drawBodyElement(std::vector<Sphere>* elements);

public:
    BatmanScene();
    void draw();
};

#endif