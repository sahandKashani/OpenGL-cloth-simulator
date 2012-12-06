#ifndef NODE_H
#define NODE_H

#include "Vector3.h"

class Node
{
private:
    Vector3 position;
    Vector3 oldPosition;
    bool moveable;
    Vector3 force;
    float mass;

public:
    Node();
    Node(Vector3 pos);
    Vector3 getPosition();
    void draw();
    bool isMoveable();
    void setMoveable(bool isMovePossible);
    void translate(Vector3 direction);
    void addForce(Vector3 extraForce);
    void applyForces(float duration);
    void setMass(float m);
    void setPosition(Vector3 pos);
    void intersectWithSpheres(Vector3 newPosition, float duration);
    void intersectWithTriangles(Vector3 newPosition);
    float triangleArea(Vector3 p1, Vector3 p2, Vector3 p3);
};

#endif