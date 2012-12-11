#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Vector3.h"
#include "Node.h"
#include "Constraint.h"
#include "StructuralConstraint.h"
#include "ShearConstraint.h"
#include "StructuralBendConstraint.h"
#include "ShearBendConstraint.h"
#include "Sphere.h"

class Cloth
{
private:
    // number of nodes in each dimension
    int numberNodesWidth;
    int numberNodesHeight;

    float clothWidth;
    float clothHeight;

    // Nodes
    std::vector< std::vector<Node> > nodes;

    // constraints
    std::vector< std::vector< std::vector<Constraint*> >* > structuralConstraints;
    std::vector< std::vector<Constraint*> > rightStructuralConstraints;
    std::vector< std::vector<Constraint*> > topStructuralConstraints;

    std::vector<Constraint> shearConstraints;
    std::vector<Constraint> upperRightShearConstraints;
    std::vector<Constraint> lowerRightShearConstraints;

    std::vector< std::vector< std::vector<Constraint*> >* > structuralBendConstraints;
    std::vector< std::vector<Constraint*> > rightStructuralBendConstraints;
    std::vector< std::vector<Constraint*> > topStructuralBendConstraints;

    std::vector<Constraint> shearBendConstraints;
    std::vector<Constraint> upperRightShearBendConstraints;
    std::vector<Constraint> lowerRightShearBendConstraints;

    // node creation method
    void createNodes();

    // constraint creation methods
    void createConstraints();
    void createStructuralConstraints();
    void createShearConstraints();
    void createStructuralBendConstraints();
    void createShearBendConstraints();

    // drawing methods
    void drawNodes();
    void drawStructuralConstraints();
    void drawShearConstraints();
    void drawStructuralBendConstraints();
    void drawShearBendConstraints();

    // constraint satisfaction methods
    void satisfyStructuralConstraints();
    void satisfyShearConstraints();
    void satisfyStructuralBendConstraints();
    void satisfyShearBendConstraints();

    void drawConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container);
    void satisfyConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container);

    // tearing methods
    // void removeRightStructuralConstraint(int x, int y);
    // void removeTopStructuralConstraint(int x, int y);

public:
    Cloth(float clothTotalWidth, float clothTotalHeight, int nodesWidth, int nodesHeight);

    // general drawing method
    void draw();

    // general constraint satisfaction method
    void satisfyConstraints();

    // force addition and application methods
    void addForce(Vector3 force);
    void applyForces(float duration);

    // getters
    int getNumberNodesWidth();
    int getNumberNodesHeight();
    float getClothWidth();
    float getClothHeight();
    Node* getNode(int x, int y);

    void handleSphereIntersections(std::vector<Sphere>* spheres);
};

#endif