#ifndef CLOTH_SIMULATOR_H
#define CLOTH_SIMULATOR_H

#include "Cloth.h"
#include "Camera.h"
#include "Floor.h"
#include "Triangle.h"
#include "Scene.h"
#include "BatmanScene.h"
#include <string>

class ClothSimulator;

class ClothSimulator
{
private:
    static ClothSimulator* instance;

    Scene* scene;

protected:
    ClothSimulator();

public:
    static ClothSimulator* getInstance();

    void draw();
    void createScene();
    void simulate();

    Scene* getScene();
};

#endif