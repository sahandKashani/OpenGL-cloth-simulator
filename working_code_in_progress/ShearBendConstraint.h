#ifndef SHEAR_BEND_CONSTRAINT_H
#define SHEAR_BEND_CONSTRAINT_H

#include "Constraint.h"
#include "Node.h"

class ShearBendConstraint : public Constraint
{
public:
    ShearBendConstraint(Node* n1, Node* n2);
    void draw();
};

#endif