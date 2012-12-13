// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Cloth.h"
#include "DrawingSettings.h"

Cloth::Cloth(float clothTotalWidth, float clothTotalHeight, int nodesWidth, int constraintInterleavingLevels) :
    clothWidth(clothTotalWidth),
    clothHeight(clothTotalHeight),
    numberNodesWidth(nodesWidth),
    numberNodesHeight(clothTotalHeight / (clothTotalWidth / nodesWidth)),
    interleaving(constraintInterleavingLevels)
{
    createNodes();
    createConstraints();
    createTriangles();
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
                // this is not a self-intersection test
                sphereIterator->handleNodeIntersection(getNode(x, y), false);
            }
        }
    }
}

void Cloth::handleSelfIntersections()
{
    for(int x1 = 0; x1 < numberNodesWidth; x1 += 1)
    {
        for(int y1 = 0; y1 < numberNodesHeight; y1 += 1)
        {
            Node* node1 = getNode(x1, y1);

            for(int x2 = 0; x2 < numberNodesWidth; x2 += 1)
            {
                for(int y2 = 0; y2 < numberNodesHeight; y2 += 1)
                {
                    // we should not test if 2 identical nodes touch each other,
                    // otherwise the cloth would deform forever, as 2 identical
                    // nodes are exactly on one another, and will try to continuously
                    // repel themselves
                    if(x1 != x2 || y1 != y2)
                    {
                        Node* node2 = getNode(x2, y2);

                        node1->handleNodeIntersection(node2);
                    }
                }
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
    float spacing = clothWidth / numberNodesWidth;

    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        float xPos = x * spacing;

        std::vector<Node> nodeColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            float yPos = y * spacing;

            // put elements in rectangular grid with 0.0 depth
            nodeColumn.push_back(Node(Vector3(xPos, yPos, 0.0), spacing / 1.125));
        }

        nodes.push_back(nodeColumn);
    }
}

void Cloth::updateTriangles()
{
    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 1; y += 1)
        {
            Vector3 bottomLeft = getNode(x, y)->getPosition();
            Vector3 bottomRight = getNode(x + 1, y)->getPosition();
            Vector3 topLeft = getNode(x, y + 1)->getPosition();
            Vector3 topRight = getNode(x + 1, y + 1)->getPosition();
            Vector3 center = (bottomLeft + bottomRight + topRight + topLeft) / 4;

            Triangle leftTriangle(center, topLeft, bottomLeft);
            Triangle bottomTriangle(center, bottomLeft, bottomRight);
            Triangle rightTriangle(center, bottomRight, topRight);
            Triangle topTriangle(center, topRight, topLeft);

            triangles[x][y][0] = leftTriangle;
            triangles[x][y][1] = bottomTriangle;
            triangles[x][y][2] = rightTriangle;
            triangles[x][y][3] = topTriangle;
        }
    }
}

void Cloth::createTriangles()
{
    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        std::vector< std::vector<Triangle> > triangleColumn;

        for(int y = 0; y < numberNodesHeight - 1; y += 1)
        {
            Vector3 bottomLeft = getNode(x, y)->getPosition();
            Vector3 bottomRight = getNode(x + 1, y)->getPosition();
            Vector3 topLeft = getNode(x, y + 1)->getPosition();
            Vector3 topRight = getNode(x + 1, y + 1)->getPosition();
            Vector3 center = (bottomLeft + bottomRight + topRight + topLeft) / 4;

            Triangle leftTriangle(center, topLeft, bottomLeft);
            Triangle bottomTriangle(center, bottomLeft, bottomRight);
            Triangle rightTriangle(center, bottomRight, topRight);
            Triangle topTriangle(center, topRight, topLeft);

            std::vector<Triangle> triangleSquare;
            triangleSquare.push_back(leftTriangle);
            triangleSquare.push_back(bottomTriangle);
            triangleSquare.push_back(rightTriangle);
            triangleSquare.push_back(topTriangle);

            triangleColumn.push_back(triangleSquare);
        }

        triangles.push_back(triangleColumn);
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
}

void Cloth::createStructuralConstraints()
{
    for(int i = 1; i <= interleaving; i += 1)
    {
        createInterleavedStructuralConstraints(i);
    }

    structuralConstraints.push_back(&rightStructuralConstraints);
    structuralConstraints.push_back(&topStructuralConstraints);
}

void Cloth::createShearConstraints()
{
    for(int i = 1; i <= interleaving; i += 1)
    {
        createInterleavedShearConstraints(i);
    }

    shearConstraints.push_back(&upperRightShearConstraints);
    shearConstraints.push_back(&lowerRightShearConstraints);
}

void Cloth::draw()
{
    drawNodes();
    drawConstraints();
    drawShaded();
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

void Cloth::drawShaded()
{
    updateTriangles();

    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 1; y += 1)
        {
            for(int triangleSquareIndex = 0; triangleSquareIndex < 4; triangleSquareIndex += 1)
            {
                triangles[x][y][triangleSquareIndex].draw();
            }
        }
    }
}

void Cloth::satisfyConstraints()
{
    satisfyStructuralConstraints();
    satisfyShearConstraints();
}

void Cloth::satisfyStructuralConstraints()
{
    satisfyConstraintsInContainer(structuralConstraints);
}

void Cloth::satisfyShearConstraints()
{
    satisfyConstraintsInContainer(shearConstraints);
}

void Cloth::drawConstraints()
{
    drawStructuralConstraints();
    drawShearConstraints();
}

void Cloth::drawStructuralConstraints()
{
    drawConstraintsInContainer(structuralConstraints);
}

void Cloth::drawShearConstraints()
{
    drawConstraintsInContainer(shearConstraints);
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

void Cloth::createInterleavedStructuralConstraints(int inter)
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        std::vector<Constraint*> rightConstraintColumn;
        std::vector<Constraint*> topConstraintColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - inter)
            {
                Node* leftNode = getNode(x, y);
                Node* rightNode = getNode(x + inter, y);
                rightConstraintColumn.push_back(new StructuralConstraint(leftNode, rightNode));
            }

            if(y < numberNodesHeight - inter)
            {
                Node* bottomNode = getNode(x, y);;
                Node* topNode = getNode(x, y + inter);
                topConstraintColumn.push_back(new StructuralConstraint(bottomNode, topNode));
            }
        }

        rightStructuralConstraints.push_back(rightConstraintColumn);
        topStructuralConstraints.push_back(topConstraintColumn);
    }
}

void Cloth::createInterleavedShearConstraints(int inter)
{
    // in x direction, only go until (numberNodesWidth - inter), because no shear constraint
    // can exist towards the right after that point
    for(int x = 0; x < numberNodesWidth - inter; x += 1)
    {
        std::vector<Constraint*> upperRightConstraintColumn;
        std::vector<Constraint*> lowerRightConstraintColumn;

        // in y direction, go until extreme top, because we have to create a lower right
        // constraint
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            Node* centerNode = getNode(x, y);

            if(y <= inter - 1)
            {
                // link to upper right node only
                Node* upperRightNode = getNode(x + inter, y + inter);
                upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
            }
            else if(y >= numberNodesHeight - inter)
            {
                // link to lower right node only
                Node* lowerRightNode = getNode(x + inter, y - inter);
                lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
            }
            else
            {
                // link to both upper right, and lower right nodes
                Node* upperRightNode = getNode(x + inter, y + inter);
                Node* lowerRightNode = getNode(x + inter, y - inter);

                upperRightConstraintColumn.push_back(new ShearConstraint(centerNode, upperRightNode));
                lowerRightConstraintColumn.push_back(new ShearConstraint(centerNode, lowerRightNode));
            }
        }

        upperRightShearConstraints.push_back(upperRightConstraintColumn);
        lowerRightShearConstraints.push_back(lowerRightConstraintColumn);
    }
}