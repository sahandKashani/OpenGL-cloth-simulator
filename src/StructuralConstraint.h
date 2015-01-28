#ifndef STRUCTURAL_CONSTRAINT_H
#define STRUCTURAL_CONSTRAINT_H

#include "Constraint.h"
#include "Node.h"

class StructuralConstraint : public Constraint
{
public:
    StructuralConstraint(Node* n1, Node* n2);
    void draw();
};

#endif