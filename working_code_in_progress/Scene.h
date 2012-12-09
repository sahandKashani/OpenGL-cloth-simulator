#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

class Scene
{
protected:
    float nearPlane;
    float farPlane;
    Camera* camera;

public:
    Scene();

    // getters
    float getNearPlane();
    float getFarPlane();
    Camera* getCamera();

    // you must not instantiate Scene, but only descendants, because they will have
    // the draw and simulate methods needed to animate themselves correctly.
    virtual void draw() = 0;
    virtual void simulate() = 0;
};

#endif