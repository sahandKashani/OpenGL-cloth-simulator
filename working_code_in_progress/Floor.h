#ifndef FLOOR_H
#define FLOOR_H

#include "Vector3.h"
#include "Node.h"

class Floor
{
private:
    Vector3 topLeft;
    Vector3 bottomLeft;
    Vector3 bottomRight;
    Vector3 topRight;
    Vector3 normal;

public:
    Floor(Vector3 tl, Vector3 bl, Vector3 br, Vector3 tr);
    void draw();
    void handleNodeIntersection(Node* node);
};

#endif