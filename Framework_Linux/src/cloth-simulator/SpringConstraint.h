#ifndef SPRING_CONTRAINT_HH
#define SPRING_CONTRAINT_HH

#include "Node.h"
#include "../math/Vector3.h"

class SpringConstraint
{

private:

    float distance_at_rest;
    Node* node_1;
    Node* node_2;

public:

    SpringConstraint(Node* n1, Node* n2);

    void satisfyConstraint();
};

#endif