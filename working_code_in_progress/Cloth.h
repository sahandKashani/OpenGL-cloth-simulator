#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Vector3.h"
#include "Node.h"
#include "Constraint.h"

class Cloth
{
private:
    // number of nodes in each dimension
    int numberNodesWidth;
    int numberNodesHeight;

    // Nodes
    std::vector< std::vector<Node> > nodes;

    std::vector<Constraint> structuralConstraints;
    std::vector<Constraint> shearConstraints;
    std::vector<Constraint> structuralBendConstraints;
    std::vector<Constraint> shearBendConstraints;

    // node creation method
    void createNodes();

    // constraint creation methods
    void createConstraints();
    void createStructuralConstraints();
    void createShearConstraints();
    void createBendConstraints();
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

public:
    Cloth();
    Cloth(int width, int height);

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
    Node* getNode(int x, int y);

    // setters
    void setNodeMass(int x, int y, float mass);
    void setNodePosition(int x, int y, Vector3 pos);
    void setNodeMoveable(int x, int y, bool moveable);
};

#endif