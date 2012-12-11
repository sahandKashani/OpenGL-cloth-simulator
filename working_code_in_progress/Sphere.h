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
    void draw();
    void handleNodeIntersection(Node* node);
    bool willHitSphere(Node* node);
    void setCenter(Vector3 c);
};

#endif