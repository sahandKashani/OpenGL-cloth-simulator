// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Cloth.h"
#include "ClothSimulator.h"

Cloth::Cloth(float clothTotalWidth, float clothTotalHeight, int nodesWidth, int nodesHeight) :
    clothWidth(clothTotalWidth),
    clothHeight(clothTotalHeight),
    numberNodesWidth(nodesWidth),
    numberNodesHeight(nodesHeight)
{
    createNodes();
    createConstraints();
}

void Cloth::handleSphereIntersections()
{
    ClothSimulator* clothSimulator = ClothSimulator::getInstance();
    for(std::vector<Sphere>::iterator sphereIterator = clothSimulator->spheres.begin();
        sphereIterator != clothSimulator->spheres.end();
        ++sphereIterator)
    {
        for(int x = 0; x < numberNodesWidth; x += 1)
        {
            for(int y = 0; y < numberNodesHeight; y += 1)
            {
                bool hitSphere = sphereIterator->handleNodeIntersection(getNode(x, y));
                if(hitSphere)
                {
                    // tearing here
                    removeRightStructuralConstraint(x, y);
                    removeTopStructuralConstraint(x, y);
                }
            }
        }
    }
}

void Cloth::removeRightStructuralConstraint(int x, int y)
{
    int position = x * (numberNodesWidth - 1) + y;
    rightStructuralConstraints[position].disable();
}

void Cloth::removeTopStructuralConstraint(int x, int y)
{
    int position = x * (numberNodesWidth - 1) + y;
    topStructuralConstraints[position].disable();
}

void removeRightStructuralBendConstraint(int x, int y)
{}

void removeTopStructuralBendConstraint(int x, int y)
{}

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
    createBendConstraints();
}

void Cloth::createStructuralConstraints()
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 1)
            {
                Node* leftNode = getNode(x, y);
                Node* rightNode = getNode(x + 1, y);
                Constraint rightConstraint(leftNode, rightNode);
                rightStructuralConstraints.push_back(rightConstraint);
            }

            if(y < numberNodesHeight - 1)
            {
                Node* bottomNode = getNode(x, y);;
                Node* topNode = getNode(x, y + 1);
                Constraint topConstraint(bottomNode, topNode);
                topStructuralConstraints.push_back(topConstraint);
            }
        }
    }

    structuralConstraints.push_back(&rightStructuralConstraints);
    structuralConstraints.push_back(&topStructuralConstraints);
}

void Cloth::createShearConstraints()
{
    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 1; y += 1)
        {
            Node* lowerLeftNode = getNode(x, y);
            Node* lowerRightNode = getNode(x + 1, y);
            Node* upperLeftNode = getNode(x, y + 1);
            Node* upperRightNode = getNode(x + 1, y + 1);

            Constraint diagonalConstraint1(lowerLeftNode, upperRightNode);
            Constraint diagonalConstraint2(lowerRightNode, upperLeftNode);

            shearConstraints.push_back(diagonalConstraint1);
            shearConstraints.push_back(diagonalConstraint2);
        }
    }
}

void Cloth::createBendConstraints()
{
    createStructuralBendConstraints();
    createShearBendConstraints();
}

void Cloth::createStructuralBendConstraints()
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 2)
            {
                Node* leftNode = getNode(x, y);
                Node* rightNode = getNode(x + 2, y);
                Constraint rightConstraint(leftNode, rightNode);
                rightStructuralBendConstraints.push_back(rightConstraint);
            }

            if(y < numberNodesHeight - 2)
            {
                Node* bottomNode = getNode(x, y);
                Node* topNode = getNode(x, y + 2);
                Constraint topConstraint(bottomNode, topNode);
                topStructuralBendConstraints.push_back(topConstraint);
            }
        }
    }
}

void Cloth::createShearBendConstraints()
{
    for(int x = 0; x < numberNodesWidth - 2; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 2; y += 1)
        {
            Node* lowerLeftNode = getNode(x, y);
            Node* lowerRightNode = getNode(x + 2, y);
            Node* upperLeftNode = getNode(x, y + 2);
            Node* upperRightNode = getNode(x + 2, y + 2);

            Constraint diagonalConstraint1(lowerLeftNode, upperRightNode);
            Constraint diagonalConstraint2(lowerRightNode, upperLeftNode);

            shearBendConstraints.push_back(diagonalConstraint1);
            shearBendConstraints.push_back(diagonalConstraint2);
        }
    }
}

void Cloth::draw()
{
    glColor3f(1.0, 1.0, 1.0);

    ClothSimulator* clothSimulator = ClothSimulator::getInstance();

    if(clothSimulator->drawNodesEnabled)
    {
        drawNodes();
    }

    if(clothSimulator->drawStructuralConstraintsEnabled)
    {
        drawStructuralConstraints();
    }

    if(clothSimulator->drawShearConstraintsEnabled)
    {
        drawShearConstraints();
    }

    if(clothSimulator->drawStructuralBendConstraintsEnabled)
    {
        drawStructuralBendConstraints();
    }

    if(clothSimulator->drawShearBendConstraintsEnabled)
    {
        drawShearBendConstraints();
    }
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

void Cloth::satisfyStructuralConstraints()
{
    for(std::vector< std::vector<Constraint>* >::iterator directionalStructuralConstraintIterator = structuralConstraints.begin();
        directionalStructuralConstraintIterator != structuralConstraints.end();
        ++directionalStructuralConstraintIterator)
    {
        for(std::vector<Constraint>::iterator structuralConstraintIterator = (*directionalStructuralConstraintIterator)->begin();
            structuralConstraintIterator != (*directionalStructuralConstraintIterator)->end();
            ++structuralConstraintIterator)
        {
            structuralConstraintIterator->satisfyConstraint();
        }
    }
}

void Cloth::satisfyShearConstraints()
{
    for(std::vector<Constraint>::iterator shearConstraintIterator = shearConstraints.begin();
        shearConstraintIterator != shearConstraints.end();
        ++shearConstraintIterator)
    {
        shearConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyStructuralBendConstraints()
{
    for(std::vector< std::vector<Constraint> >::iterator directionalStructuralBendConstraintIterator = structuralBendConstraints.begin();
        directionalStructuralBendConstraintIterator != structuralBendConstraints.end();
        ++directionalStructuralBendConstraintIterator)
    {
        for(std::vector<Constraint>::iterator structuralBendConstraintIterator = directionalStructuralBendConstraintIterator->begin();
            structuralBendConstraintIterator != directionalStructuralBendConstraintIterator->end();
            ++structuralBendConstraintIterator)
        {
            structuralBendConstraintIterator->satisfyConstraint();
        }
    }
}

void Cloth::satisfyShearBendConstraints()
{
    for(std::vector<Constraint>::iterator shearBendConstraintIterator = shearBendConstraints.begin();
        shearBendConstraintIterator != shearBendConstraints.end();
        ++shearBendConstraintIterator)
    {
        shearBendConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyConstraints()
{
    satisfyStructuralConstraints();
    satisfyShearConstraints();
    satisfyStructuralBendConstraints();
    satisfyShearBendConstraints();
}

void Cloth::drawStructuralConstraints()
{
    for(std::vector< std::vector<Constraint>* >::iterator directionalStructuralConstraintIterator = structuralConstraints.begin();
        directionalStructuralConstraintIterator != structuralConstraints.end();
        ++directionalStructuralConstraintIterator)
    {
        for(std::vector<Constraint>::iterator structuralConstraintIterator = (*directionalStructuralConstraintIterator)->begin();
            structuralConstraintIterator != (*directionalStructuralConstraintIterator)->end();
            ++structuralConstraintIterator)
        {
            structuralConstraintIterator->draw();
        }
    }
}

void Cloth::drawShearConstraints()
{
    for(std::vector<Constraint>::iterator shearConstraintIterator = shearConstraints.begin();
        shearConstraintIterator != shearConstraints.end();
        ++shearConstraintIterator)
    {
        shearConstraintIterator->draw();
    }
}

void Cloth::drawStructuralBendConstraints()
{
    for(std::vector< std::vector<Constraint> >::iterator directionalStructuralBendConstraintIterator = structuralBendConstraints.begin();
        directionalStructuralBendConstraintIterator != structuralBendConstraints.end();
        ++directionalStructuralBendConstraintIterator)
    {
        for(std::vector<Constraint>::iterator structuralBendConstraintIterator = directionalStructuralBendConstraintIterator->begin();
            structuralBendConstraintIterator != directionalStructuralBendConstraintIterator->end();
            ++structuralBendConstraintIterator)
        {
            structuralBendConstraintIterator->draw();
        }
    }
}

void Cloth::drawShearBendConstraints()
{
    for(std::vector<Constraint>::iterator shearBendConstraintIterator = shearBendConstraints.begin();
        shearBendConstraintIterator != shearBendConstraints.end();
        ++shearBendConstraintIterator)
    {
        shearBendConstraintIterator->draw();
    }
}