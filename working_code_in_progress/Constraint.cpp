#include "Constraint.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void Constraint::draw()
{
    if(enabled)
    {
        Vector3 firstNodePosition = node1->getPosition();
        Vector3 secondNodePosition = node2->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

Constraint::Constraint(Node* n1, Node* n2) :
    node1(n1),
    node2(n2),
    distanceAtRest((n1->getPosition() - n2->getPosition()).length()),
    enabled(true)
{}

void Constraint::disable()
{
    enabled = false;
}

Node* Constraint::getFirstNode()
{
    return node1;
}

Node* Constraint::getSecondNode()
{
    return node2;
}

float Constraint::getDistanceAtRest()
{
    return distanceAtRest;
}

void Constraint::satisfyConstraint()
{
    if(enabled)
    {
        Vector3 vectorFromNode1ToNode2 = node2->getPosition() - node1->getPosition();
        float currentDistance = vectorFromNode1ToNode2.length();
        float restToCurrentDistanceRatio = distanceAtRest / currentDistance;
        Vector3 correctionVectorFromNode1ToNode2 = vectorFromNode1ToNode2 * (1 - restToCurrentDistanceRatio);

        bool node1Moveable = node1->isMoveable();
        bool node2Moveable = node2->isMoveable();

        if(node1Moveable && node2Moveable)
        {
            // move both nodes towards each other by 0.5 * correctionVectorFromNode1ToNode2
            // positive direction for node1 (correction vector goes from node1 to node 2)
            // therefore, negative direction for node2
            node1->translate(0.5 * correctionVectorFromNode1ToNode2);
            node2->translate(-0.5 * correctionVectorFromNode1ToNode2);
        }
        else if(node1Moveable && !node2Moveable)
        {
            // move node1 towards node2 by +1.0 * correctionVectorFromNode1ToNode2
            // (positive sign, because the correction vector is pointing towards node2)
            node1->translate(correctionVectorFromNode1ToNode2);
        }
        else if(!node1Moveable && node2Moveable)
        {
            // move node2 towards node1 by -1.0 * correctionVectorFromNode1ToNode2
            // (negative sign, because the correction vector is pointing towards node2)
            node2->translate(-correctionVectorFromNode1ToNode2);
        }
    }
}