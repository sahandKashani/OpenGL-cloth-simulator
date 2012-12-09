#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

class Scene
{
protected:
    float nearPlane;
    float farPlane;
    Camera camera;

public:
    Scene();
    float getNearPlane();
    float getFarPlane();

    void draw();
};

#endif