#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Vector3.h"
#include "Node.h"
#include "Constraint.h"
#include "StructuralConstraint.h"
#include "ShearConstraint.h"
#include "Sphere.h"

class Cloth
{
private:
    // number of nodes in each dimension
    int numberNodesWidth;
    int numberNodesHeight;

    float clothWidth;
    float clothHeight;

    int interleaving;

    // Nodes
    std::vector< std::vector<Node> > nodes;

    // structural constraints
    std::vector< std::vector< std::vector<Constraint*> >* > structuralConstraints;
    std::vector< std::vector<Constraint*> > rightStructuralConstraints;
    std::vector< std::vector<Constraint*> > topStructuralConstraints;

    // shear constraints
    std::vector< std::vector< std::vector<Constraint*> >* > shearConstraints;
    std::vector< std::vector<Constraint*> > upperRightShearConstraints;
    std::vector< std::vector<Constraint*> > lowerRightShearConstraints;

    // node creation method
    void createNodes();

    // constraint creation methods
    void createConstraints();
    void createStructuralConstraints();
    void createShearConstraints();

    void createTriangles();

    void createInterleavedStructuralConstraints(int inter);
    void createInterleavedShearConstraints     (int inter);

    // drawing methods
    void drawNodes();
    void drawConstraints();
    void drawStructuralConstraints();
    void drawShearConstraints();
    void drawShaded();

    // constraint satisfaction methods
    void satisfyStructuralConstraints();
    void satisfyShearConstraints();

    void drawConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container);
    void satisfyConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container);

public:
    Cloth(float clothTotalWidth, float clothTotalHeight, int nodesWidth, int constraintInterleavingLevels);

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
    void handleSelfIntersections();
};

#endif