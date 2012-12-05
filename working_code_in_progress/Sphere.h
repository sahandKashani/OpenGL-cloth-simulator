#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Node.h"

class Sphere
{
private:
    Vector3 center;
    float radius;

public:
    Sphere(Vector3 c, float r);
    Vector3 getCenter();
    float getRadius();
    void handleIntersection(Node* node);
    void draw();
};

#endif