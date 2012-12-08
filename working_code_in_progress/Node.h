#ifndef NODE_H
#define NODE_H

#include "Vector3.h"

class Node
{
private:
    Vector3 position;
    Vector3 oldPosition;
    bool moveable;
    Vector3 originalForce;
    Vector3 force;
    float mass;

public:
    Node();
    Node(Vector3 pos);

    Vector3 getPosition();
    Vector3 getOldPosition();
    Vector3 getForce();

    void setMoveable(bool isMovePossible);
    void setMass(float m);
    void setPosition(Vector3 pos);
    void setForce(Vector3 f);

    bool isMoveable();
    void translate(Vector3 direction);

    void draw();

    void addForce(Vector3 extraForce);
    void applyForces(float duration);

    void resetToOriginalForce();
};

#endif