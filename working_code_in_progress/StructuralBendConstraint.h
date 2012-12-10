#ifndef STRUCTURAL_BEND_CONSTRAINT
#define STRUCTURAL_BEND_CONSTRAINT

#include "Constraint.h"
#include "Node.h"

class StructuralBendConstraint : public Constraint
{
public:
    StructuralBendConstraint(Node* n1, Node* n2);
    void draw();
};

#endif