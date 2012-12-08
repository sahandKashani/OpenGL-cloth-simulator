#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Node.h"

class Constraint
{
private:
    Node* node1;
    Node* node2;
    float distanceAtRest;

public:
    Constraint();
    Constraint(Node* n1, Node* n2);
    Node* getFirstNode();
    Node* getSecondNode();
    float getDistanceAtRest();
    void satisfyConstraint();
    void draw();
};

#endif