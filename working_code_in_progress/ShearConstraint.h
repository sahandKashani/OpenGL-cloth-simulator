#ifndef SHEAR_CONSTRAINT_H
#define SHEAR_CONSTRAINT_H

#include "Constraint.h"
#include "Node.h"

class ShearConstraint : public Constraint
{
public:
    ShearConstraint(Node* n1, Node* n2);
    void draw();
};

#endif