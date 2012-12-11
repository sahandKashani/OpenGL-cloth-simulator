// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Cloth.h"
#include "DrawingSettings.h"

Cloth::Cloth(float clothTotalWidth, float clothTotalHeight, int nodesWidth, int nodesHeight) :
    clothWidth(clothTotalWidth),
    clothHeight(clothTotalHeight),
    numberNodesWidth(nodesWidth),
    numberNodesHeight(nodesHeight)
{
    createNodes();
    createConstraints();
}

void Cloth::handleSphereIntersections(std::vector<Sphere>* spheres)
{
    for(std::vector<Sphere>::iterator sphereIterator = spheres->begin();
        sphereIterator != spheres->end();
        ++sphereIterator)
    {
        for(int x = 0; x < numberNodesWidth; x += 1)
        {
            for(int y = 0; y < numberNodesHeight; y += 1)
            {
                sphereIterator->handleNodeIntersection(getNode(x, y));
            }
        }
    }
}

float Cloth::getClothWidth()
{
    return clothWidth;
}

float Cloth::getClothHeight()
{
    return clothHeight;
}

Node* Cloth::getNode(int x, int y)
{
    return &nodes[x][y];
}

int Cloth::getNumberNodesWidth()
{
    return numberNodesWidth;
}

int Cloth::getNumberNodesHeight()
{
    return numberNodesHeight;
}

void Cloth::createNodes()
{
    float horizontalSpacing = clothWidth / numberNodesWidth;
    float verticalSpacing = clothHeight / numberNodesHeight;

    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        float xPos = x * horizontalSpacing;

        std::vector<Node> nodeColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            float yPos = y * verticalSpacing;

            // put elements in rectangular grid with 0.0 depth
            nodeColumn.push_back(Node(Vector3(xPos, yPos, 0.0)));
        }

        nodes.push_back(nodeColumn);
    }
}

// moves the nodes depending on the forces that are being applied to them
void Cloth::applyForces(float duration)
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            getNode(x, y)->applyForces(duration);
        }
    }
}

void Cloth::addForce(Vector3 force)
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            getNode(x, y)->addForce(force);
        }
    }
}

void Cloth::createConstraints()
{
    createStructuralConstraints();
    createShearConstraints();
    createStructuralBendConstraints();
    createShearBendConstraints();
}

void Cloth::createStructuralConstraints()
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        std::vector<Constraint*> rightConstraintColumn;
        std::vector<Constraint*> topConstraintColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 1)
            {
                Node* leftNode = getNode(x, y);
                Node* rightNode = getNode(x + 1, y);
                rightConstraintColumn.push_back(new StructuralConstraint(leftNode, rightNode));
            }

            if(y < numberNodesHeight - 1)
            {
                Node* bottomNode = getNode(x, y);;
                Node* topNode = getNode(x, y + 1);
                topConstraintColumn.push_back(new StructuralConstraint(bottomNode, topNode));
            }
        }

        rightStructuralConstraints.push_back(rightConstraintColumn);
        topStructuralConstraints.push_back(topConstraintColumn);
    }

    structuralConstraints.push_back(&rightStructuralConstraints);
    structuralConstraints.push_back(&topStructuralConstraints);
}

void Cloth::createShearConstraints()
{
    // in x direction, only go until numberNodesWidth - 1, because no shear constraint
    // can exist towards the right after that point
    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        std::vector<Constraint*> upperRightConstraintColumn;
        std::vector<Constraint*> lowerRightConstraintColumn;

        // in y direction, go until extreme top, because we have to create a lower right
        // constraint
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            Node* centerNode = getNode(x, y);

            if(y == 0)
            {
                // link to upper right node only
                Node* upperRightNode = getNode(x + 1, y + 1);
                upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
            }
            else if(y == numberNodesHeight - 1)
            {
                // link to lower right node only
                Node* lowerRightNode = getNode(x + 1, y - 1);
                lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
            }
            else
            {
                // link to both upper right, and lower right nodes
                Node* upperRightNode = getNode(x + 1, y + 1);
                Node* lowerRightNode = getNode(x + 1, y - 1);

                upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
                lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
            }
        }

        upperRightShearConstraints.push_back(upperRightConstraintColumn);
        lowerRightShearConstraints.push_back(lowerRightConstraintColumn);
    }

    shearConstraints.push_back(&upperRightShearConstraints);
    shearConstraints.push_back(&lowerRightShearConstraints);
}

void Cloth::createStructuralBendConstraints()
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        std::vector<Constraint*> rightBendConstraintColumn;
        std::vector<Constraint*> topBendConstraintColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 2)
            {
                Node* leftNode = getNode(x, y);
                Node* rightNode = getNode(x + 2, y);
                rightBendConstraintColumn.push_back(new StructuralBendConstraint(leftNode, rightNode));
            }

            if(y < numberNodesHeight - 2)
            {
                Node* bottomNode = getNode(x, y);;
                Node* topNode = getNode(x, y + 2);
                topBendConstraintColumn.push_back(new StructuralBendConstraint(bottomNode, topNode));
            }
        }

        rightStructuralBendConstraints.push_back(rightBendConstraintColumn);
        topStructuralBendConstraints.push_back(topBendConstraintColumn);
    }

    structuralBendConstraints.push_back(&rightStructuralBendConstraints);
    structuralBendConstraints.push_back(&topStructuralBendConstraints);
}

void Cloth::createShearBendConstraints()
{
    // // for(int x = 0; x < numberNodesWidth - 2; x += 1)
    // // {
    // //     for(int y = 0; y < numberNodesHeight - 2; y += 1)
    // //     {
    // //         Node* lowerLeftNode = getNode(x, y);
    // //         Node* lowerRightNode = getNode(x + 2, y);
    // //         Node* upperLeftNode = getNode(x, y + 2);
    // //         Node* upperRightNode = getNode(x + 2, y + 2);

    // //         Constraint diagonalConstraint1(lowerLeftNode, upperRightNode);
    // //         Constraint diagonalConstraint2(lowerRightNode, upperLeftNode);

    // //         shearBendConstraints.push_back(diagonalConstraint1);
    // //         shearBendConstraints.push_back(diagonalConstraint2);
    // //     }
    // // }

    // // in x direction, only go until numberNodesWidth - 1, because no shear constraint
    // // can exist towards the right after that point
    // for(int x = 0; x < numberNodesWidth - 2; x += 1)
    // {
    //     std::vector<Constraint*> upperRightConstraintColumn;
    //     std::vector<Constraint*> lowerRightConstraintColumn;

    //     // in y direction, go until extreme top, because we have to create a lower right
    //     // constraint
    //     for(int y = 0; y < numberNodesHeight; y += 1)
    //     {
    //         Node* centerNode = getNode(x, y);

    //         if(y == 0)
    //         {
    //             // link to upper right node only
    //             Node* upperRightNode = getNode(x + 2, y + 2);
    //             upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
    //         }
    //         else if(y == numberNodesHeight - 1)
    //         {
    //             // link to lower right node only
    //             Node* lowerRightNode = getNode(x + 2, y - 2);
    //             lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
    //         }
    //         else
    //         {
    //             // link to both upper right, and lower right nodes
    //             Node* upperRightNode = getNode(x + 2, y + 2);
    //             Node* lowerRightNode = getNode(x + 2, y - 2);

    //             upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
    //             lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
    //         }
    //     }

    //     upperRightShearBendConstraints.push_back(upperRightConstraintColumn);
    //     lowerRightShearBendConstraints.push_back(lowerRightConstraintColumn);
    // }

    // shearBendConstraints.push_back(&upperRightShearBendConstraints);
    // shearBendConstraints.push_back(&lowerRightShearBendConstraints);
}

void Cloth::draw()
{
    drawNodes();
    drawConstraints();
}

void Cloth::drawNodes()
{
    // loop over vertices (not edges)
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            getNode(x, y)->draw();
        }
    }
}

void Cloth::satisfyConstraints()
{
    satisfyStructuralConstraints();
    satisfyShearConstraints();
    satisfyStructuralBendConstraints();
    satisfyShearBendConstraints();
}

void Cloth::satisfyStructuralConstraints()
{
    satisfyConstraintsInContainer(structuralConstraints);
}

void Cloth::satisfyShearConstraints()
{
    satisfyConstraintsInContainer(shearConstraints);
}

void Cloth::satisfyStructuralBendConstraints()
{
    satisfyConstraintsInContainer(structuralBendConstraints);
}

void Cloth::satisfyShearBendConstraints()
{
    satisfyConstraintsInContainer(shearBendConstraints);
}

void Cloth::drawConstraints()
{
    drawStructuralConstraints();
    drawShearConstraints();
    drawStructuralBendConstraints();
    drawShearBendConstraints();
}

void Cloth::drawStructuralConstraints()
{
    drawConstraintsInContainer(structuralConstraints);
}

void Cloth::drawShearConstraints()
{
    drawConstraintsInContainer(shearConstraints);
}

void Cloth::drawStructuralBendConstraints()
{
    drawConstraintsInContainer(structuralBendConstraints);
}

void Cloth::drawShearBendConstraints()
{
    drawConstraintsInContainer(shearBendConstraints);
}

// method for automatic drawing of constraints in a container
void Cloth::drawConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container)
{
    // iterating over std::vector< std::vector< std::vector<Constraint*> >* >
    for(std::vector< std::vector< std::vector<Constraint*> >* >::iterator it1 = container.begin();
        it1 != container.end();
        ++it1)
    {
        // iterating over std::vector< std::vector<Constraint*> >*
        for(std::vector< std::vector<Constraint*> >::iterator it2 = (*it1)->begin();
            it2 != (*it1)->end();
            ++it2)
        {
            // iterating over std::vector<Constraint*>
            for(std::vector<Constraint*>::iterator it3 = it2->begin();
                it3 != it2->end();
                ++it3)
            {
                (*it3)->draw();
            }
        }
    }
}

// method for automatic satisfaction of constraints in a container
void Cloth::satisfyConstraintsInContainer(std::vector< std::vector< std::vector<Constraint*> >* > container)
{
    // iterating over std::vector< std::vector< std::vector<Constraint*> >* >
    for(std::vector< std::vector< std::vector<Constraint*> >* >::iterator it1 = container.begin();
        it1 != container.end();
        ++it1)
    {
        // iterating over std::vector< std::vector<Constraint*> >*
        for(std::vector< std::vector<Constraint*> >::iterator it2 = (*it1)->begin();
            it2 != (*it1)->end();
            ++it2)
        {
            // iterating over std::vector<Constraint*>
            for(std::vector<Constraint*>::iterator it3 = it2->begin();
                it3 != it2->end();
                ++it3)
            {
                (*it3)->satisfyConstraint();
            }
        }
    }
}