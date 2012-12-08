#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Node.h"

class Sphere
{
private:
    Vector3 center;
    float radius;
    bool sticky;

public:
    Sphere(Vector3 c, float r);
    Sphere(Vector3 c, float r, bool stick);
    Vector3 getCenter();
    float getRadius();
    void draw();
    bool handleNodeIntersection(Node* node);
};

#endif